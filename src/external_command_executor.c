#include <stdlib.h>
#include <string.h>
#include "external_command_executor.h"

void execute_external_command(char* command) {
    if (strcmp(command, "\\q\n") == 0) {
        exit(EXIT_SUCCESS);
    }
}