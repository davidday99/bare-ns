#include "tcp.h"
#include "ipv4.h"

void tcp_send(uint32_t destip, uint8_t *data, uint16_t len) {
    ipv4_send(destip, data, len, IPV4_PROTOCOL_TCP);
}