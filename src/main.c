#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command_reader.h"
#include "external_command_executor.h"
#include "history.h"
#include "command_parser.h"
#include "command_executor.h"

int main(void) {
    setbuf(stdout, NULL);
    init_history_buffer();
    load_commands_from_file();

    char* command = NULL;
    char** parsed_command = NULL;

    while (1) {
        command = read_command();

        if (strcmp(command, "\\q") == 0) {
            exit(EXIT_SUCCESS);
        }

        if (*command) {
            add_command_to_file(command);

            CommandStatus status = execute_external_command(command);

            if (status == CMD_UNKNOWN) {
                parsed_command = parse_command(command);
                execute_command(parsed_command);

                free(parsed_command);
            }
        }

        free(command);
    }

    return EXIT_SUCCESS;
}