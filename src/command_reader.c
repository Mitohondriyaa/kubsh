#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "command_reader.h"

char* read_command(void) {
    char* command = readline("\033[1;34mKubSH> \033[0m");

    if (!command) {
        exit(EXIT_SUCCESS);
    }

    if (*command) {
        add_history(command);
    }

    return command;
}