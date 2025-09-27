#include <stdio.h>
#include <stdlib.h>
#include "command_reader.h"

char* read_command(void) {
    char* buffer = NULL;
    size_t buffer_size = 0;

    if (getline(&buffer, &buffer_size, stdin) == -1) {
        if (feof(stdin)) {
            printf("\n");
            exit(EXIT_SUCCESS);
        }

        fprintf(stderr, "\033[1;31mStdin error\033[0m");
        exit(EXIT_FAILURE);
    }

    return buffer;
}