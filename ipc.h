#include <stdint.h>

void init_ipc(int fd[]);
void send_data(int8_t writepipe, uint8_t *data, uint32_t bytes);
uint32_t recv_data(int8_t readpipe, uint8_t *data);