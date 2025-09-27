#include <stdio.h>
#include "command_reader.h"

char* read_command(void) {
    char* buffer = NULL;
    size_t buffer_size = 0;

    getline(&buffer, &buffer_size, stdin);

    return buffer;
}