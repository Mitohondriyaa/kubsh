#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include "inttypes.h"
#include "disk_parser.h"

PartitionStyle detect_partition_style(char* disk) {
    int fd = open(disk, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "\033[1;31mDisk access error\n\033[0m");

        return UNKNOWN;
    }

    unsigned char sector[SECTOR_SIZE];

    if (read(fd, sector, SECTOR_SIZE) != SECTOR_SIZE) {
        fprintf(stderr, "\033[1;31mDisk reading error\n\033[0m");

        close(fd);

        return UNKNOWN;
    }
   
    close(fd);
    
    if (sector[0x1FE] != 0x55 || sector[0x1FF] != 0xAA) {
        fprintf(stderr, "\033[1;31mDisk reading error\033[0m");
        
        return UNKNOWN;
    }

    if (sector[0x1BE + 4] == 0xEE) {
        return GPT;
    }
    else {
        return MBR;
    }
}

CommandStatus print_mbr_partition_info(char* disk) {
    int fd = open(disk, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "\033[1;31mDisk access error\n\033[0m");

        return CMD_ERROR;
    }

    unsigned char sector[SECTOR_SIZE];

    if (read(fd, sector, SECTOR_SIZE) != SECTOR_SIZE) {
        fprintf(stderr, "\033[1;31mDisk reading error\n\033[0m");

        close(fd);

        return CMD_ERROR;
    }
   
    close(fd);

    for (int i = 0; i < 4; i++) {
        unsigned char* partition = &sector[0x1BE + 16 * i];
        unsigned char status = partition[0];
        unsigned char type = partition[4];

        if (type == 0) {
            continue;
        }

        uint32_t lba_start = *(uint32_t*) &partition[8];
        uint32_t num_sectors = *(uint32_t*) &partition[12];

        printf(
            "Partition %d: status=0x%02X, type=0x%02X, start=%u, sectors=%u, size=%u MiB\n",
             i+1, status, type, lba_start, num_sectors, num_sectors / 2048
        ); 
    }

    return CMD_OK;
}

CommandStatus print_gpt_partition_info(char* disk) {
    int fd = open(disk, O_RDONLY);

    if (fd < 0) {
        fprintf(stderr, "\033[1;31mDisk access error\n\033[0m");

        return CMD_ERROR;
    }

    unsigned char sector[SECTOR_SIZE];

    if (lseek(fd, SECTOR_SIZE, SEEK_SET) < 0 ||  read(fd, sector, SECTOR_SIZE) != SECTOR_SIZE) {
        fprintf(stderr, "\033[1;31mDisk reading error\n\033[0m");

        close(fd);

        return CMD_ERROR;
    }

    uint32_t num_partition_entries = *(uint32_t*) &sector[0x50];
    uint32_t size_of_partition_entry = *(uint32_t*)&sector[0x54];
    uint64_t first_entry_lba = *(uint64_t*) &sector[0x48];
    unsigned char* table = malloc(num_partition_entries * size_of_partition_entry);

    if (!table) {
        fprintf(stderr, "\033[1;31mAllocation error\033[0m");
        
        return CMD_ERROR;
    }

        if (
            lseek(fd, first_entry_lba * SECTOR_SIZE, SEEK_SET) < 0
            || read(fd, table, num_partition_entries * size_of_partition_entry) != num_partition_entries * size_of_partition_entry
        ) {
            fprintf(stderr, "\033[1;31mDisk reading error\n\033[0m");

            close(fd);

            return CMD_ERROR;
        }

    for (uint32_t i = 0; i < num_partition_entries; i++) {
        unsigned char* entry = table + i * size_of_partition_entry;

        int empty = 1;

        for (int j = 0; j < 16; j++) {
            if (entry[j] != 0) {
                empty = 0;
                break;
            }
        }

        if (!empty) {
            uint64_t first_lba = *(uint64_t*)&entry[32];
            uint64_t last_lba  = *(uint64_t*)&entry[40];

            printf(
                "Partition %d: first LBA=%" PRIu64 ", last LBA=%" PRIu64 ", size=%" PRIu64 " MiB\n",
                 i+1, first_lba, last_lba, (last_lba - first_lba + 1) / 2048
            );
        }
    }

    free(table);
   
    close(fd);

    return CMD_OK;
}