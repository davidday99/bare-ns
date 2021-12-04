#ifndef _ARP_H_
#define _ARP_H_

#include <stdint.h>

struct arp_pkt {
    uint16_t hw_type;
    uint16_t protocol_type;
    uint8_t hwlen;
    uint8_t plen;
    uint16_t opcode;
    uint8_t *hwaddr_sender;
    uint8_t *paddr_sender;
    uint8_t *hwaddr_target;
    uint8_t *paddr_target;
} __attribute__((packed));

#define ARP_RX_BUF_LEN 10
#define ARP_TX_BUF_LEN 10

#define ARES_OP_REQUEST 1
#define ARES_OP_REPLY 2
#define ARES_HRD_ETHERNET 1

void init_arp_pkt(struct arp_pkt *a, uint8_t *data);
uint8_t write_rx_arp_pkt(struct arp_pkt *a);
struct arp_pkt *read_rx_arp_pkt();

#endif /* _ARP_H_ */