#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "external_command_executor.h"

CommandStatus execute_external_command(char* command) {
    if (strncmp(command, "echo", 4) == 0 && (command[4] == ' ' || command[4] == '\0')) {
        char* to_print = command[4] == ' ' ? command + 5 : "";

        printf("%s\n", to_print);

        return CMD_OK;
    }

    if (strncmp(command, "\\e $", 4) == 0) {
        char* var = command + 4;

        if (strlen(var) == 0) {
            fprintf(stderr, "\033[1;31mEnvironment variable is empty\n\033[0m");

            return CMD_ERROR;
        }

        char* value = getenv(var);

        if (!value) {
            fprintf(stderr, "\033[1;31mEnvironment variable does not exist\033[0m\n");
            
            return CMD_ERROR;
        }

        char* copy = strdup(value);
        char* token = strtok(copy, ":");

        while (token) {
            printf("%s\n", token);
            token = strtok(NULL, ":");
        }

        free(copy);

        return CMD_OK;
    }

    return CMD_UNKNOWN;
}