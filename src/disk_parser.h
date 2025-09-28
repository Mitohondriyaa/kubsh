#ifndef DISK_PARSER_H
#define DISK_PARSER_H

#include "command.h"

#define SECTOR_SIZE 512

typedef enum {
    MBR,
    GPT,
    UNKNOWN
} PartitionStyle;

PartitionStyle detect_partition_style(char* disk);
CommandStatus print_mbr_partition_info(char* disk);
CommandStatus print_gpt_partition_info(char* disk);

#endif