#include <stdio.h>
#include <stdlib.h>
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

        if (*command) {
            add_command_to_file(command);

            execute_external_command(command);

            printf("%s\n", command);
        }
    }

    free(command);

    return EXIT_SUCCESS;
}