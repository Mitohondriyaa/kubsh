#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <readline/history.h>
#include "history.h"

HistoryBuffer history_buffer;

void init_history_buffer(void) {
    if (!history_buffer.buffer) {
        history_buffer.buffer = malloc(sizeof(char*) * MAX_COMMANDS);
    
        if (!history_buffer.buffer) {
            fprintf(stderr, "\033[1;31mAllocation error033[0m");
            exit(EXIT_FAILURE);
        }
    }
    else {
        for (int i = 0; i < history_buffer.size; i++) {
            free(history_buffer.buffer[(history_buffer.start + i) % MAX_COMMANDS]);
            history_buffer.buffer[(history_buffer.start + i) % MAX_COMMANDS] = NULL;
        }
    }
}

char* get_history_filename(void) {
    char* home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "\033[1;31mError getting HOME variable\033[0m");
        exit(EXIT_FAILURE);
    }

    char* filename = ".kubsh_history";
    size_t length = strlen(home) + strlen(filename) + 2;
    char* history_filename = malloc(length);

    if (!history_filename) {
        fprintf(stderr, "\033[1;31mAllocation error\033[0m");
        exit(EXIT_FAILURE);
    }

    snprintf(history_filename, length, "%s/%s", home, filename);

    return history_filename;
}

void add_command_to_buffer(char* command) {
    if (history_buffer.size < MAX_COMMANDS) {
        int idx = (history_buffer.start + history_buffer.size) % MAX_COMMANDS;
        history_buffer.buffer[idx] = strdup(command);
        history_buffer.size++;
    }
    else {
        free(history_buffer.buffer[history_buffer.start]);
        history_buffer.buffer[history_buffer.start] = strdup(command);
        history_buffer.start = (history_buffer.start + 1) % MAX_COMMANDS;
    }
}

void add_command_to_file(char* command) {
    char* history_filename = get_history_filename();

    if (access(history_filename, F_OK)) {
        init_history_buffer();
    }

    FILE* history_file = fopen(history_filename, "w");

    if (!history_file) {
        fprintf(stderr, "\033[1;31mFile open error\033[0m");
        exit(EXIT_FAILURE);
    }

    add_command_to_buffer(command);

    for (int i = 0; i < history_buffer.size; i++) {
        int idx = (history_buffer.start + i) % MAX_COMMANDS;
        fprintf(history_file, "%s\n", history_buffer.buffer[idx]);
    }

    fclose(history_file);
    free(history_filename);
}

void load_commands_from_file(void) {
    char* history_filename = get_history_filename();

    if (!access(history_filename, F_OK)) {
        FILE* history_file = fopen(history_filename, "r");

        if (!history_file) {
            fprintf(stderr, "\033[1;31mFile open error\033[0m");
            exit(EXIT_FAILURE);
        }   

        char* command = NULL;
        size_t size = 0;

        while (getline(&command, &size, history_file) != -1) {
            size_t length = strlen(command);

            command[length - 1] = '\0';

            add_history(command);
            add_command_to_buffer(command);
        }

        fclose(history_file);
        free(command);
    }

    free(history_filename);
}