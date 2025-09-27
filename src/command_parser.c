#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_parser.h"

char** parse_command(char* command) {
    char** parsed_command = NULL;
    char* delimiter = " ";
    char* command_copy = strdup(command);

    if (!command_copy) {
        fprintf(stderr, "\033[1;31mAllocation error\033[0m");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(command_copy, delimiter);
    int count = 0;

    while (token) {
        token = strtok(NULL, delimiter);
        count++;
    }

    free(command_copy);

    parsed_command = malloc(sizeof(char*) * (count + 1));

    if (!parsed_command) {
        fprintf(stderr, "\033[1;31mAllocation error\033[0m");
        exit(EXIT_FAILURE);
    }

    token = strtok(command, delimiter);

    for (int i = 0; token; i++) {
        parsed_command[i] = token;
        token = strtok(NULL, delimiter);
    }

    parsed_command[count] = NULL;

    return parsed_command;
}