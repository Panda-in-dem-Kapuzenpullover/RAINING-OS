/*
* RAINING-OS Secure Update System
 * Copyright (C) 2026 Macaulay Smith
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <glob.h>
#include <sys/wait.h>

extern char **environ;

const char *public_key =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAxWhx7fKamavVJpdAFZ2i\n"
    "8gryMG/u5UQ+s5zYJvMfLzR4ZRf6vcyYOpjQbOKUL2lCWBVTrpOTjUh8ib6ZMZbU\n"
    "W4/BCtxDwzsDbqRLFqlYhS0cCGEJw0RymBzMdLEfLE+w6qB63pvnYZ9WcNijU16w\n"
    "YfiUiRflN+3/81Kdx/mH5tqdMT4ZIYAYc1zi/SWmMgqEQsQ/unt4Ta6eYqmC8pYY\n"
    "mztiQLZALG46WkdFc5P2n7JmeeZDSG5ZNJ0bcx2Ed0apyXJ2Ydppf14nedFHOGdH\n"
    "YhUabLA2aNKktzCchUmbrVwkySJKIx1QqVAR6KsZ/rBY5ndiKUVVlXqb94Kij3Ci\n"
    "0JTIIUM81pS3yphp+IYkrGqKazuDg+2Q55KCmYc3glNMs7UCB9dt3WedeH/isFbn\n"
    "Jb0vYNzaV2hbNpRBPgehpdl1FNodQZQB2GXtf/693DiDw7E9pn0Kz234ZHetl5ZD\n"
    "lwg/DF+h+0p44bhsRt0vD4Z2OqcuhMfBOKNwfPu4H9RDo/D2dIExzZfpvhJrplNH\n"
    "NiSlA4csc2fZ00x25SsWACX231IaYrbEMHtNwHnLurLkjdH/DJHKMfJ4kZbvrD+/\n"
    "KRIR8hBVkdLRjFt5/fvRDY6jL/137FA11BL6nyk/nxh+46O3975KljrMDe1iX29U\n"
    "17TtCP4uiLTK0aweM0fI02sCAwEAAQ==\n"
    "-----END PUBLIC KEY-----\n";

char* find_file(const char *pattern, char *buffer, size_t buf_size) {
    glob_t glob_result;
    if (glob(pattern, GLOB_NOSORT, NULL, &glob_result) != 0) return NULL;
    if (glob_result.gl_pathc == 0) {
        globfree(&glob_result);
        return NULL;
    }
    strncpy(buffer, glob_result.gl_pathv[0], buf_size - 1);
    buffer[buf_size - 1] = '\0';
    globfree(&glob_result);
    return buffer;
}

int verify_signature(const char *binary_file, const char *sig_file) {
    char cmd[4096];

    snprintf(cmd, sizeof(cmd),
        "echo \"%s\" | openssl dgst -sha256 -verify /dev/stdin -signature '%s' '%s' 2>/dev/null",
        public_key, sig_file, binary_file);

    int result = system(cmd);
    return (result == 0);
}

int extract_and_verify(const char *package_file) {
    char temp_dir[] = "/tmp/update_XXXXXX";
    int final_success = 1;
    umask(077);

    if (!mkdtemp(temp_dir)) {
        perror("Failed to create secure temp directory");
        return 1;
    }

    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "tar -xzf '%s' -C '%s' 2>/dev/null", package_file, temp_dir);
    if (system(cmd) != 0) {
        fprintf(stderr, "Failed to extract package\n");
        goto cleanup;
    }

    char binary_file[512], sig_file[512], pattern[512];
    snprintf(pattern, sizeof(pattern), "%s/*.sig", temp_dir);
    if (!find_file(pattern, sig_file, sizeof(sig_file))) {
        fprintf(stderr, "No signature file found\n");
        goto cleanup;
    }

    strncpy(binary_file, sig_file, sizeof(binary_file) - 1);
    binary_file[strlen(binary_file) - 4] = '\0';

    printf("Verifying package signature against embedded Root of Trust...\n");
    if (!verify_signature(binary_file, sig_file)) {
        fprintf(stderr, "❌ ERROR: Signature mismatch! Verification failed.\n");
        goto cleanup;
    }

    printf("✓ Signature verified successfully!\n");
    chmod(binary_file, 0755);


    const char *secure_path = "/usr/local/bin/raining_secured_update";
    if (rename(binary_file, secure_path) != 0) {
        perror("Failed to secure binary (rename)");
        goto cleanup;
    }

    printf("Executing secured update...\n");
    int exec_result = system(secure_path);


    if (exec_result == -1) {
        perror("❌ Failed to start the update process");
    } else {
        if (WIFEXITED(exec_result)) {
            int exit_status = WEXITSTATUS(exec_result);
            if (exit_status == 0) {
                printf("✓ Update completed successfully!\n");
                final_success = 0;
            } else if (exit_status == 1) {
                printf("⚠️ Update finished with warnings (Exit Code 1). Check logs.\n");
                final_success = 0;
            } else {
                fprintf(stderr, "❌ Update failed with error code: %d\n", exit_status);
            }
        } else if (WIFSIGNALED(exec_result)) {
            fprintf(stderr, "❌ FATAL: Update process crashed via signal %d\n", WTERMSIG(exec_result));
        }
    }

cleanup:
    snprintf(cmd, sizeof(cmd), "rm -rf '%s'", temp_dir);
    system(cmd);
    return final_success;
}

int main(int argc, char *argv[]) {

#ifdef __APPLE__
    static char *empty_env[] = { NULL };
    environ = empty_env;
#else
    clearenv();
#endif

    setenv("PATH", "/usr/bin:/bin:/usr/sbin:/sbin", 1);

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <update_package.raining>\n", argv[0]);
        return 1;
    }

    const char *package = argv[1];
    const char *dot = strrchr(package, '.');
    if (!dot || strcmp(dot, ".raining") != 0) {
        fprintf(stderr, "Error: Invalid file extension. Raining-OS requires .raining packages.\n");
        return 1;
    }

    printf("=== Raining-OS Secure Update System ===\n");
    return extract_and_verify(package);
}
