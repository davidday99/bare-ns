#include <stdint.h>
#include "udp.h"
#include "ipv4.h"
#include "netcommon.h"
#include "string.h"
#include "checksum.h"

void udp_send(uint16_t srcport, uint32_t destip, uint16_t destport, uint8_t *data, uint16_t len) {
    uint8_t udpsegment[UDP_PSEUDO_HEADER_SIZE + UDP_HEADER_SIZE + len];
    uint16_t total_udpsegment_len = len + UDP_HEADER_SIZE;
    
    uint32_t *ptr_four_bytes = (uint32_t *) udpsegment;
    uint16_t *ptr_two_bytes;
    *ptr_four_bytes++ = hton32(ipv4_get_address());
    *ptr_four_bytes++ = hton32(destip);
    ptr_two_bytes = (uint16_t *) ptr_four_bytes;
    *ptr_two_bytes++ = hton16(0x0011);
    *ptr_two_bytes++ = hton16(total_udpsegment_len);

    struct udphdr *hdr = (struct udphdr *) &udpsegment[UDP_PSEUDO_HEADER_SIZE];
    hdr->srcport = hton16(srcport);
    hdr->destport = hton16(destport);
    hdr->len = hton16(total_udpsegment_len);
    hdr->cksm = 0;
    memcpy(udpsegment + UDP_PSEUDO_HEADER_SIZE + UDP_HEADER_SIZE, data, len);

    hdr->cksm = ~ones_complement_sum(udpsegment, sizeof(udpsegment));
    
    ipv4_send(destip, &udpsegment[UDP_PSEUDO_HEADER_SIZE], total_udpsegment_len, IPV4_PROTOCOL_UDP);
}
