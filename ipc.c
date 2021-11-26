#include "ipc.h"
#include <unistd.h>
#include <fcntl.h>

void send_data(char *pathname, uint8_t *data, uint32_t bytes) {
    uint8_t fd = open(pathname, O_CREAT| O_RDWR | O_APPEND, 0777);
    write(fd, &bytes, 4);
    write(fd, data, bytes);
    close(fd);
}

uint32_t recv_data(char *pathname, uint8_t *data) {
    uint32_t len;

    uint8_t fd = open(pathname, O_CREAT| O_RDONLY, 0777);

    read(fd, &len, 4);
    read(fd, data, len);

    return len;
}