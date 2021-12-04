#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include <stdint.h>

#define ENET_RX_BUF_LEN 10
#define ENET_TX_BUF_LEN 10

#define ENET_SRC_LEN 6
#define ENET_DEST_LEN 6
#define ENET_TYPE_LEN 6
#define ENET_FCS_LEN 4

#define ETYPE_IP 0x0800
#define ETYPE_ARP 0x0806

struct enet_frame {
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
    uint16_t dlen;
    uint8_t data[1500];
    uint8_t fcs[4];
};

uint8_t write_rx_frame(struct enet_frame *e);
uint8_t write_tx_frame(struct enet_frame *e);
struct enet_frame *read_rx_frame();
struct enet_frame *read_tx_frame();
void init_frame(struct enet_frame *e, uint8_t *buf, uint16_t dlen);

#endif /* _ETHERNET_H_ */