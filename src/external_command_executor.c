#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "external_command_executor.h"

void execute_external_command(char* command) {
    if (strcmp(command, "\\q") == 0) {
        exit(EXIT_SUCCESS);
    }

    if (strncmp(command, "echo", 4) == 0 && (command[4] == ' ' || command[4] == '\0')) {
        char* to_print = command[4] == ' ' ? command + 5 : "";

        printf("%s\n", to_print);
    }
}