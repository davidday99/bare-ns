#include <stdint.h>

uint8_t tx_buf[2000];

void net_tx(uint8_t *destmac, uint8_t *data, uint16_t len, uint16_t type) {
    memcpy(tx_buf, data, len);
}