#include <stdint.h>
#include "ipv4.h"
#include "tcp.h"
#include "tcp.h"
#include "udp.h"
#include "netcommon.h"

void ipv4_deliver(uint8_t *buf) {
    struct ipv4hdr *hdr = (struct ipv4hdr *) buf;
    uint8_t options_len = ipv4_options_len(hdr);
    uint8_t *payload = (uint8_t *) &((uint8_t  *) hdr)[IPV4_MIN_HEADER_LEN + options_len];

    uint8_t phdrbuf[sizeof(struct pseudohdr)];
    struct pseudohdr *phdr = (struct pseudohdr *) phdrbuf;
    phdr->srcip = hdr->src;
    phdr->destip = hdr->dest;
    phdr->zero = 0;
    phdr->pctl = hdr->protocol;
    phdr->len = hton16(ipv4_data_len(hdr));

    switch (hdr->protocol) {
        case IPV4_PROTOCOL_TCP:
            tcp_deliver(payload, phdrbuf);
            break;
        case IPV4_PROTOCOL_UDP:
            udp_deliver(payload, hton32(hdr->src), hton32(hdr->dest), ipv4_data_len(hdr));
            break;
        case IPV4_PROTOCOL_ICMP:
            // icmp_handle()
            break;
        default:
            break;
    }
}
