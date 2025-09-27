#ifndef EXTERNAL_COMMAND_EXECUTOR_H
#define EXTERNAL_COMMAND_EXECUTOR_H

typedef enum {
    CMD_OK,
    CMD_UNKNOWN,
    CMD_ERROR
} CommandStatus;

CommandStatus execute_external_command(char* command);

#endif