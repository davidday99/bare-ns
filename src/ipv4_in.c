#include <stdint.h>
#include "ipv4.h"
#include "tcp.h"

void ipv4_deliver(struct ipv4hdr *pkt) {
    switch (pkt->protocol) {
        case IPV4_PROTOCOL_TCP:
            // tcp_deliver()
            break;
        case IPV4_PROTOCOL_UDP:
            // udp_deliver()
            break;
        case IPV4_PROTOCOL_ICMP:
            // icmp_handle()
            break;
        default:
            break;
    }
}