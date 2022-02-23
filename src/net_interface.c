#include <stdint.h>

const uint8_t MAC_BROADCAST = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uint8_t GATEWAY_MAC[6] = {0,0,0,0,0,0};

__attribute__ ((weak)) void net_tx(uint8_t *destmac, uint8_t *data, uint16_t len, uint16_t type) {
    return;
}

__attribute__ ((weak)) void net_rx(uint8_t *buf) {
    return;
}

__attribute__ ((weak)) void net_mac_address(uint8_t *buf) {
    return;
}