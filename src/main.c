#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_reader.h"
#include "external_command_executor.h"
#include "history.h"

int main(void) {
    setbuf(stdout, NULL);
    init_history_buffer();
    load_commands_from_file();

    char* command = NULL;

    while (1) {
        command = read_command();

        if (strcmp(command, "\\q") == 0) {
            exit(EXIT_SUCCESS);
        }

        if (*command) {
            add_command_to_file(command);

            CommandStatus status = execute_external_command(command);

            if (status == CMD_UNKNOWN) {
                fprintf(stderr, "\033[1;31mUnknown command: %s\n\033[0m", command);
            }
        }
    }

    free(command);

    return EXIT_SUCCESS;
}