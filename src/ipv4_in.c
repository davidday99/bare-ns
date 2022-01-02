#include <stdint.h>
#include "ipv4_in.h"
#include "ipv4.h"
#include "tcp.h"
#include "tcp_in.h"
#include "udp_in.h"
#include "netcommon.h"

void ipv4_deliver(struct ipv4hdr *pkt) {
    uint8_t options_len = ipv4_options_len(pkt);
    uint8_t *payload = (uint8_t *) &((uint8_t  *) pkt)[IPV4_MIN_HEADER_LEN + options_len];

    switch (pkt->protocol) {
        case IPV4_PROTOCOL_TCP:
            tcp_deliver(payload, hton32(pkt->src), hton32(pkt->dest), ipv4_data_len(pkt));
            break;
        case IPV4_PROTOCOL_UDP:
            udp_deliver(payload, hton32(pkt->src), hton32(pkt->dest), ipv4_data_len(pkt));
            break;
        case IPV4_PROTOCOL_ICMP:
            // icmp_handle()
            break;
        default:
            break;
    }
}
