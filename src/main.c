#include <stdio.h>
#include <stdlib.h>
#include "command_reader.h"

int main(void) {
    setbuf(stdout, NULL);

    char* command = NULL;

    while (1) {
        printf("\033[1;34mKubSH> \033[0m");

        command = read_command();

        printf("%s", command);

        free(command);
    }

    return EXIT_SUCCESS;
}