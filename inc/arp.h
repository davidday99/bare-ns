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
};

#define ARP_RX_BUF_LEN 10
#define ARP_TX_BUF_LEN 10

#define ARES_OP_REQUEST 1
#define ARES_OP_REPLY 2
#define ARES_HRD_ETHERNET 1

#endif /* _ARP_H_ */