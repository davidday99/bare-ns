#include "tcp.h"
#include "ipv4.h"
#include "netcommon.h"
#include "string.h"

uint16_t calculate_tcp_checksum(uint8_t *data, uint16_t len);
static uint16_t ones_complement_sum(uint8_t *data, uint16_t len);

void tcp_send(uint32_t destip, uint8_t *data, uint16_t len) {
    uint8_t pseudohdr[sizeof(struct tcppseudohdr) + len];
    struct tcppseudohdr *phdr = (struct tcppseudohdr *) pseudohdr;
    struct tcphdr *hdr = (struct tcphdr *) data;

    phdr->srcip = hton32(ipv4_get_address());
    phdr->destip = hton32(destip);
    phdr->zero = 0;
    phdr->pctl = 6;
    phdr->len = hton16(len);
    memcpy(&pseudohdr[sizeof(struct tcppseudohdr)], data, len);
    hdr->cksm = calculate_tcp_checksum(pseudohdr, sizeof(pseudohdr));

    ipv4_send(destip, data, len, IPV4_PROTOCOL_TCP);
}

uint16_t calculate_tcp_checksum(uint8_t *data, uint16_t len) {
    uint16_t sum = ones_complement_sum(data, len);
    sum = ~sum;
    return sum;
}

static uint16_t ones_complement_sum(uint8_t *data, uint16_t len) {
    uint16_t *ptr_two_bytes = (uint16_t *) data;
    uint32_t overflow;
    uint32_t sum = 0;
    
    for (uint16_t i = 0; i < len / 2; i++) {
        sum += *ptr_two_bytes++;
    }
    
    if (len & 1) {
        sum += *ptr_two_bytes & 0xFF;
    }
    
    while ((overflow = (sum & 0xFFFF0000))) {
        overflow >>= 16;
        sum = (sum & 0xFFFF) + overflow;
    }
    
    return sum;
}