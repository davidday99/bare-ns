#ifndef _IP_H_
#define _IP_H_

#include <stdint.h>
#include "ethernet.h"

#define IP_RX_BUF_LEN 10
#define IP_TX_BUF_LEN 10

struct ip_pkt {
    uint8_t dest[4];
    uint8_t src[4];
    uint8_t protocol;
    uint8_t *payload;
    uint16_t plen;
};

uint8_t write_rx_pkt(struct enet_frame *e);
struct ip_pkt *read_rx_pkt();

#endif /* _IP_H_ */