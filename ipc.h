#include <stdint.h>

void send_data(char *pathname, uint8_t *data, uint32_t bytes);
uint32_t recv_data(char *pathname, uint8_t *data);