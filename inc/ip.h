#ifndef _IP_H_
#define _IP_H_

#include <stdint.h>
#include "ethernet.h"

#define IP_RX_BUF_LEN 10
#define IP_TX_BUF_LEN 10

struct ip_pkt {
    uint32_t header1;
    uint32_t header2;
    uint32_t header3;
    uint32_t src;
    uint32_t dest;
    uint8_t *options;
    uint8_t *data;
    uint8_t olen;
    uint16_t dlen;
};

uint8_t write_rx_ip_pkt(struct ip_pkt *p);
struct ip_pkt *read_rx_ip_pkt();
void init_ip_pkt(struct ip_pkt *p, uint8_t *buf);

#endif /* _IP_H_ */