#include <stdint.h>

__attribute__ ((weak)) void net_tx(uint8_t *destmac, uint8_t *data, uint16_t len, uint16_t type) {
    return;
}

__attribute__ ((weak)) void net_rx(uint8_t *buf) {
    return;
}

