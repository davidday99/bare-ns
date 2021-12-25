#ifndef _ICMP_H_
#define _ICMP_H_

#include <stdint.h>

struct icmphdr {
    uint8_t type;
    uint8_t code;
    uint16_t chksm;
};

void icmp_request(struct icmphdr *hdr);
void icmp_reply(struct icmphdr *hdr);

#endif /* _ICMP_H_ */