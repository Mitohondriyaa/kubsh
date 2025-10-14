#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "command_executor.h"

void execute_command(char** parsed_command) {
    pid_t pid = fork();

    if (pid == 0) {
        execvp(parsed_command[0], parsed_command);

        fprintf(stdout, "%s: command not found\n", parsed_command[0]);
        
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) {
        fprintf(stderr, "\033[1;31mFork error\033[0m\n");

        exit(EXIT_FAILURE);
    }
    else {
        wait(NULL);
    }
}