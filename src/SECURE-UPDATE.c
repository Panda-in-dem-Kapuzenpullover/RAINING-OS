
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

#define PUBLIC_KEY_PATH "/etc/CRYPTOGRAPHY/public_key.pem"

char* find_file(const char *pattern, char *buffer, size_t buf_size) {
    glob_t glob_result;

    if (glob(pattern, GLOB_NOSORT, NULL, &glob_result) != 0) {
        return NULL;
    }

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
    if (access(binary_file, R_OK) != 0) {
        fprintf(stderr, "Binary file not accessible: %s\n", binary_file);
        return 0;
    }
    if (access(sig_file, R_OK) != 0) {
        fprintf(stderr, "Signature file not accessible: %s\n", sig_file);
        return 0;
    }

    char cmd[1024];
    snprintf(cmd, sizeof(cmd),
        "openssl dgst -sha256 -verify '%s' -signature '%s' '%s' 2>/dev/null",
        PUBLIC_KEY_PATH, sig_file, binary_file);

    int result = system(cmd);
    return (result == 0);
}

int extract_and_verify(const char *package_file) {
    char temp_dir[] = "/tmp/update_XXXXXX";
    if (!mkdtemp(temp_dir)) {
        perror("Failed to create temp directory");
        return 1;
    }

    char extract_cmd[1024];
    snprintf(extract_cmd, sizeof(extract_cmd),
        "tar -xzf '%s' -C '%s' 2>/dev/null", package_file, temp_dir);

    if (system(extract_cmd) != 0) {
        fprintf(stderr, "Failed to extract package\n");
        char cleanup[256];
        snprintf(cleanup, sizeof(cleanup), "rm -rf '%s'", temp_dir);
        system(cleanup);
        return 1;
    }

    char binary_file[512], sig_file[512];
    char pattern[512];

    snprintf(pattern, sizeof(pattern), "%s/*.sig", temp_dir);
    if (!find_file(pattern, sig_file, sizeof(sig_file))) {
        fprintf(stderr, "No signature file found in package\n");
        char cleanup[256];
        snprintf(cleanup, sizeof(cleanup), "rm -rf '%s'", temp_dir);
        system(cleanup);
        return 1;
    }

    strncpy(binary_file, sig_file, sizeof(binary_file) - 1);
    binary_file[strlen(binary_file) - 4] = '\0';

    if (access(binary_file, R_OK) != 0) {
        fprintf(stderr, "Binary file not found: %s\n", binary_file);
        char cleanup[256];
        snprintf(cleanup, sizeof(cleanup), "rm -rf '%s'", temp_dir);
        system(cleanup);
        return 1;
    }

    printf("Verifying package signature...\n");
    if (!verify_signature(binary_file, sig_file)) {
        fprintf(stderr, "❌ ERROR: Invalid signature! Package may be tampered with.\n");
        char cleanup[256];
        snprintf(cleanup, sizeof(cleanup), "rm -rf '%s'", temp_dir);
        system(cleanup);
        return 1;
    }

    printf("✓ Signature verified successfully!\n");

    chmod(binary_file, 0755);

    printf("Executing update...\n");
    char exec_cmd[1024];
    snprintf(exec_cmd, sizeof(exec_cmd),
        "sudo '%s'", binary_file);
    int exec_result = system(exec_cmd);

    if (exec_result == 0) {
        printf("✓ Update completed successfully!\n");
    } else {
        fprintf(stderr, "❌ Update failed with exit code: %d\n", exec_result);
    }

    char cleanup[256];
    snprintf(cleanup, sizeof(cleanup), "rm -rf '%s'", temp_dir);
    system(cleanup);

    return (exec_result == 0) ? 0 : 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <update_package.raining>\n", argv[0]);
        return 1;
    }

    if (access(PUBLIC_KEY_PATH, R_OK) != 0) {
        fprintf(stderr, "Error: Public key not found at %s\n", PUBLIC_KEY_PATH);
        return 1;
    }

    const char *package = argv[1];

    if (access(package, R_OK) != 0) {
        fprintf(stderr, "Error: Cannot access file: %s\n", package);
        return 1;
    }

    // FIXED: More robust extension check
    const char *dot = strrchr(package, '.');
    
    // Debug output
    fprintf(stderr, "DEBUG: package='%s', len=%zu\n", package, strlen(package));
    if (dot) {
        fprintf(stderr, "DEBUG: extension='%s', len=%zu\n", dot, strlen(dot));
    } else {
        fprintf(stderr, "DEBUG: no extension found\n");
    }
    
    if (!dot) {
        fprintf(stderr, "Error: File has no extension\n");
        return 1;
    }
    
    if (strcmp(dot, ".raining") != 0) {
        fprintf(stderr, "Error: Not a valid .raining file (extension is '%s')\n", dot);
        return 1;
    }

    printf("=== Secure Update System ===\n");
    printf("Package: %s\n\n", package);

    return extract_and_verify(package);
}
