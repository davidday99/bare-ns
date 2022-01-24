#include <stdint.h>
#include "ipv4.h"
#include "net.h"
#include "netcommon.h"
#include "string.h"
#include "ethernet.h"
#include "checksum.h"

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