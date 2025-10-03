#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void expand_tilde_in_command(char** command) {
    char* tilde = NULL;

    while((tilde = strchr(*command, '~')) != NULL) {
        char* expanded = NULL;
        char* home = getenv("HOME");

        if (!home) {
            fprintf(stderr, "\033[1;31mError getting HOME variable\033[0m");
            exit(EXIT_FAILURE);
        }

        size_t length = strlen(home) + strlen(*command);

        expanded = malloc(length);

        size_t prefix_length = tilde - *command;

        strncpy(expanded, *command, prefix_length);
        expanded[prefix_length] = '\0';
        strcat(expanded, home);
        strcat(expanded, tilde + 1);

        free(*command);

        *command = expanded;
    }
}