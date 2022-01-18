#include <stdint.h>
#include "ipv4.h"
#include "net.h"
#include "netcommon.h"
#include "string.h"
#include "ethernet.h"

static uint16_t ones_complement_sum(uint8_t *data, uint16_t len);

void ipv4_send(uint32_t destip, uint8_t *data, uint16_t len, uint8_t protcol) {
    uint8_t ipv4pkt[IPV4_MIN_HEADER_LEN + len];
    struct ipv4hdr *hdr = (struct ipv4hdr *) ipv4pkt;
    hdr->version = 4;
    hdr->ihl = 5;
    hdr->tos = 0;
    hdr->len = hton16(IPV4_MIN_HEADER_LEN + len);
    hdr->id = hton16(0xFFFF);
    hdr->frag_offset = hton16(0x4000);
    hdr->ttl = 64;
    hdr->protocol = protcol;
    hdr->cksm = 0;
    hdr->dest = hton32(destip);
    hdr->src = hton32(ipv4_get_address());
    hdr->cksm = ~ones_complement_sum(ipv4pkt, IPV4_MIN_HEADER_LEN);
    memcpy(ipv4pkt + IPV4_MIN_HEADER_LEN, data, len);
    net_tx(0, ipv4pkt, sizeof(ipv4pkt), ETHERTYPE_IPV4);
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