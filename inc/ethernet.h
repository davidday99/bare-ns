#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include <stdint.h>

#define ENET_RX_BUF_LEN 1518
#define ENET_TX_BUF_LEN 1518

#define ENET_HEADER_SIZE 14
#define ENET_TYPE_OFFSET 12
#define ENET_DATA_OFFSET 14

#define ETHERTYPE_IPV4 0x0800
#define ETHERTYPE_ARP 0x0806

struct enethdr {
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
} __attribute__((packed));

extern uint8_t enet_rx_waiting;
extern uint8_t enet_tx_waiting;

uint16_t ethernet_handle_frame(uint8_t *rx_buf);
void ethernet_write_tx_buffer(struct enethdr *hdr, uint8_t *data, uint16_t len);
void ethernet_read_tx_buffer(uint8_t *buf);

#endif /* _ETHERNET_H_ */