#include "ipc.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void init_ipc(int fd[]) {
    pipe(fd);
}

void send_data(int8_t writepipe, uint8_t *data, uint32_t bytes) {
    write(writepipe, &bytes, 4);
    write(writepipe, data, bytes);
}

uint32_t recv_data(int8_t readpipe, uint8_t *data) {
    uint32_t len;
    read(readpipe, &len, 4);
    read(readpipe, data, len);
    return len;
}
