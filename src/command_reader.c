#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "command_reader.h"
#include "signal_handler.h"

char* read_command(void) {
    char* command = NULL;

    while (1) {
        command = readline("\033[1;34mKubSH> \033[0m");

        if (!command) {
            if (sighup_received) {
                sighup_received = 0;

                continue;
            }
            else {
                exit(EXIT_SUCCESS);
            }
        }

        break;
    }

    if (*command) {
        add_history(command);
    }

    return command;
}