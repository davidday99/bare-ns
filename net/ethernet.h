#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include <stdint.h>

#define RX_BUF_LEN 10
#define TX_BUF_LEN 10

struct enet_frame {
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
    uint16_t dlen;
    uint8_t data[1500];
    uint8_t fcs[4];
};

uint8_t write_rx_frame(uint8_t *buf, uint16_t dlen);
struct enet_frame *read_rx_frame();

#endif /* _ETHERNET_H_ */