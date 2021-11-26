#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ipc.h"

#define BUF "buf"

void unknown_cmd();
void read_cmd(uint8_t *data);
void write_cmd(char *data, uint32_t len);
void exit_cmd();

uint8_t get_command(char *data) {
    char cmd[20];
    uint8_t ccode;
    scanf("%s", cmd);
    if (strcmp(cmd, "quit") == 0) {
        ccode = 0;
    } else if (strcmp(cmd, "read") == 0) {
        ccode = 1;
    } else if (strcmp(cmd, "write") == 0) {
        ccode = 2;
        scanf("%s", data);
    } else {
        ccode = 3;
    }
    return ccode;
}

void execute_command(uint8_t ccode, uint8_t *data, uint32_t len) {
    switch (ccode) {
        case 0:
            exit_cmd();
            break;
        case 1:
            read_cmd(data);
            break;
        case 2:
            write_cmd(data, len);
            break;
        default:
            unknown_cmd();
            break;
    }
}

void exit_cmd() {
    printf("Exiting.\n");
    exit(0);
}

void unknown_cmd() {
    printf("Uknown command.\n");
}

void read_cmd(uint8_t *data) {
    uint8_t len;
    len = recv_data(BUF, data);
    printf("read: ");
    for (uint8_t i = 0; i < len; i++)
        printf("%c", data[i]);
    printf("\n");
}

void write_cmd(char *data, uint32_t len) {
    send_data(BUF, data, len);
}

int main(char *argv[], int argc) {

    char data[20];
    uint8_t ccode;
    uint32_t len;
    
    while (1) {
        ccode = get_command(data);
        len = strlen(data);
        execute_command(ccode, data, len);
    }

    return 0;
}