#include <stdint.h>
#include "udp.h"
#include "ipv4.h"
#include "netcommon.h"
#include "string.h"

uint16_t calculate_udp_checksum(uint32_t srcip, uint32_t destip, struct udphdr *hdr, uint8_t *data, uint16_t len);

void udp_send(uint16_t srcport, uint32_t destip, uint16_t destport, uint8_t *data, uint16_t len) {
    uint8_t udpsegment[UDP_HEADER_SIZE + len];
    struct udphdr *hdr = (struct udphdr *) udpsegment;
    hdr->srcport = hton16(srcport);
    hdr->destport = hton16(destport);
    hdr->cksm = hton16(calculate_udp_checksum(ipv4_get_address(), destip, hdr, data, len));
    
    memcpy(udpsegment + UDP_HEADER_SIZE, data, len);

    ipv4_send(destip, udpsegment, sizeof(udpsegment), IPV4_PROTOCOL_UDP);
}

uint16_t calculate_udp_checksum(uint32_t srcip, uint32_t destip, struct udphdr *hdr, uint8_t *data, uint16_t len) {
    uint32_t total_len = len + UDP_PSEUDO_HEADER_SIZE + UDP_HEADER_SIZE;

    uint32_t sum = 0;

    return sum;
}