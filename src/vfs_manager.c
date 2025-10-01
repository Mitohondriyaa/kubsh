#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include "vfs_manager.h"

char* init_mountpoint() {
    char* home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "\033[1;31mError getting HOME variable\033[0m");
        exit(EXIT_FAILURE);
    }

    size_t length = strlen(home) + 7;
    char* mountpoint = malloc(length);

    if (!mountpoint) {
        fprintf(stderr, "\033[1;31mAllocation error\033[0m");
        exit(EXIT_FAILURE);
    }

    snprintf(mountpoint, length, "%s/users", home);

    if (mkdir(mountpoint, 0755) != 0) {
        if (errno != EEXIST) {
            fprintf(stderr, "\033[1;31mFailed to create mountpoint\033[0m");
            exit(EXIT_FAILURE);
        }
    }

    return mountpoint;
}

