#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
void setup_wizard() {
    char decision;
    printf("Would you like to continue? [Y/N] ");
    scanf(" %c", &decision);

    if (decision == 'Y' || decision == 'y') {
        if (getuid() != 0) {
            printf("Rerun as root\n");
            exit(0);
        }
        else {
            printf("Installing prerequisites...\n");
            int result_one = system("apt install -y qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virt-manager python3-pip");
            if (result_one != 0) {
                fprintf(stderr, "Error: Package installation failed.\n");
                exit(1);
            }

            printf("Cloning OSX-KVM repository...\n");
            int result = system("git clone --depth 1 --recursive https://github.com/kholia/OSX-KVM.git");
            if (result != 0) {
                fprintf(stderr, "Error: Git clone failed with code %d\n", result);
                exit(1);
            }


            if (chdir("OSX-KVM") != 0) {
                perror("Error: Failed to change directory to OSX-KVM");
                exit(1);
            }

            char answer;
            printf("\nWould you like to download macOS? [Y/N] ");
            scanf(" %c", &answer);

            if (answer == 'Y' || answer == 'y') {
                printf("This will download macOS from Apple's servers.\n");
                printf("Apple's EULA requires macOS to run on Apple-branded hardware.\n");
                printf("By continuing, you agree to comply with Apple's terms.\n\n");

                printf("Fetching macOS recovery image...\n");
                system("python3 fetch-macOS-v2.py");

                char whenurdonehb[10];
                printf("\nType 'DONE' when you're finished selecting a version: ");
                scanf("%s", whenurdonehb);

                if (strcmp(whenurdonehb, "DONE") == 0) {
                    printf("Converting recovery image...\n");
                    int conv_result = system("qemu-img convert BaseSystem.dmg -O raw BaseSystem.img");
                    if (conv_result != 0) {
                        fprintf(stderr, "Error: Image conversion failed\n");
                        exit(1);
                    }

                    printf("Creating virtual disk (128GB)...\n");
                    int disk_result = system("qemu-img create -f qcow2 mac_hdd_ng.img 128G");
                    if (disk_result != 0) {
                        fprintf(stderr, "Error: Failed to create virtual disk\n");
                        exit(1);
                    }

                    printf("\n=== Setup Almost done! ===\n");
                    printf("\nConfiguring boot script...\n");
                    printf("Enter RAM to allocate in MB (recommended 8192+): ");
                    int ram;
                    scanf("%d", &ram);

                    printf("Enter CPU cores (recommended 4): ");
                    int cores;
                    scanf("%d", &cores);

                    // Modify the script
                    char sed_cmd[256];
                    sprintf(sed_cmd, "sed -i 's/ALLOCATED_RAM=\"4096\"/ALLOCATED_RAM=\"%d\"/' OpenCore-Boot.sh", ram);
                    system(sed_cmd);

                    sprintf(sed_cmd, "sed -i 's/CPU_CORES=\"2\"/CPU_CORES=\"%d\"/' OpenCore-Boot.sh", cores);
                    system(sed_cmd);

                    // Make it executable
                    system("chmod +x OpenCore-Boot.sh");

                    printf("\n=== Setup Complete! ===\n");
                    printf("To start the VM, run: ./OpenCore-Boot.sh\n");
                }
            }
        }
    } else {
        printf("Setup cancelled.\n");
    }
}

int main(void) {
    printf("=== macOS KVM Setup Wizard ===\n\n");
    setup_wizard();
    return 0;
}
