#ifndef _UDP_H_
#define _UDP_H_

#include <stdint.h>

struct udphdr {
    uint16_t srcport;
    uint16_t destport;
    uint16_t len;
    uint16_t cksm;
};

void udp_deliver(uint8_t *payload, uint32_t srcip, uint32_t destip, uint16_t len);
void udp_send(uint16_t srcport, uint32_t destip, uint16_t destport, uint8_t *data, uint16_t len);

#define UDP_HEADER_SIZE 8
#define UDP_PSEUDO_HEADER_SIZE 12

#endif /* _UDP_H_ */