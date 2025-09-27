#ifndef HISTORY_H
#define HISTORY_H

#define MAX_COMMANDS 10

typedef struct {
    char** buffer;
    int start;
    int size;
} HistoryBuffer;

extern HistoryBuffer history_buffer;

void init_history_buffer(void);
void add_command_to_file(char* command);
void load_commands_from_file(void);

#endif