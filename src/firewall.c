/*
* Copyright (C) 2026  [Macaulay Smith (PANDA-MIT-DEM-KAPUZENPULLOVER)]
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/*
Yes, this is an extremely simple project...
I'm not even gonna bother putting comments on it.
*/
    int main(int argc, char *argv[]) {
        if (argc < 2) {
            fprintf(stderr, "Usage: %s <1|2|3>\n", argv[0]);
            return 1;
        }
        if (geteuid() != 0) {
            fprintf(stderr, "Error: Use sudo.\n");
            return EXIT_FAILURE;
        }

        int decision = atoi(argv[1]);

    if (decision == 1) {
        system("ufw default deny incoming && ufw default deny outgoing && ufw allow out 53");
    }
    else if (decision == 2) {
        system("ufw --force reset");
        system("ufw default deny incoming");
        system("ufw allow out 53");
        system("ufw allow out 80,443/tcp");
        system ("ufw allow out 123/udp");
        system ("ufw allow out 67,68/udp");
    }
    else if (decision == 3) {
    printf("Extremely dangerous, do not enable\n");
        system("ufw disable");
    }
    else {
        exit(1);
    }
    if (decision == 1 || decision == 2) {
        system("ufw --force enable");
    }
    return 0;
}
