#ifndef _IPV4_H_
#define _IPV4_H_

#include <stdint.h>

struct ipv4hdr {
    uint8_t ihl : 4;
    uint8_t version : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t frag_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t cksm;
    uint32_t src;
    uint32_t dest;
};

void icmp_reply(struct ipv4hdr *hdr);

#endif /* _IPV4_H_ */