#include <stdio.h>
#include <stdlib.h>
#include "command_reader.h"
#include "external_command_executor.h"

int main(void) {
    setbuf(stdout, NULL);

    char* command = NULL;

    while (1) {
        command = read_command();

        if (*command) {
            execute_external_command(command);

            printf("%s\n", command);
        }

        free(command);
    }

    return EXIT_SUCCESS;
}