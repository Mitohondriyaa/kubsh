#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "external_command_executor.h"
#include "disk_parser.h"

CommandStatus execute_external_command(char* command) {
    if (strncmp(command, "echo", 4) == 0 && (command[4] == ' ' || command[4] == '\0')) {
        char* to_print = command[4] == ' ' ? command + 5 : "";

        if (to_print[0] == '\'' && to_print[strlen(to_print) - 1] == '\'') {
            to_print[strlen(to_print) - 1] = '\0';
            to_print++;
        }
        
        printf("%s\n", to_print);

        return CMD_OK;
    }

    if (strncmp(command, "\\e $", 4) == 0) {
        char* var = command + 4;

        if (strlen(var) == 0) {
            fprintf(stderr, "\033[1;31mEnvironment variable is empty\n\033[0m");

            return CMD_ERROR;
        }

        char* value = getenv(var);

        if (!value) {
            fprintf(stderr, "\033[1;31mEnvironment variable does not exist\033[0m\n");
            
            return CMD_ERROR;
        }

        char* copy = strdup(value);
        char* token = strtok(copy, ":");

        while (token) {
            printf("%s\n", token);
            token = strtok(NULL, ":");
        }

        free(copy);

        return CMD_OK;
    }

    if (strncmp(command, "\\l", 2) == 0) {
        char* disk_sufix = command + 2;

        while (*disk_sufix == ' ') {
            disk_sufix++;
        }

        if (strlen(disk_sufix) == 0) {
            fprintf(stderr, "\033[1;31mDisk is not selected\n\033[0m");

            return CMD_ERROR;
        }

        size_t length = strlen(disk_sufix) + 6;
        char* disk = malloc(length);

        snprintf(disk, length, "/dev/%s", disk_sufix);

        PartitionStyle partition_style = detect_partition_style(disk);
        CommandStatus status;

        switch (partition_style) {
            case MBR:
                status = print_mbr_partition_info(disk);
                break;
            case GPT:
                status = print_gpt_partition_info(disk);
                break;
            default:
                status = CMD_ERROR;
                break;
        }

        free(disk);

        return status;
    }

    if (strncmp(command, "cd", 2) == 0) {
        if (*(command + 2) == ' ' && *(command + 3) != '\0') {
            if (chdir(command + 3) != 0) {
                fprintf(stderr, "\033[1;31mDirectory is not found\n\033[0m");

                return CMD_ERROR;
            }

            return CMD_OK;
        }
        
        fprintf(stderr, "\033[1;31mDirectory is not selected\n\033[0m");

        return CMD_ERROR;
    }

    return CMD_UNKNOWN;
}