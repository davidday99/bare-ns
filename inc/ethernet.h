#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#include <stdint.h>

#define ENET_RX_BUF_LEN 1518
#define ENET_TX_BUF_LEN 1518

#define ENET_HEADER_DEST_LEN 6
#define ENET_HEADER_SRC_LEN 6
#define ENET_HEADER_TYPE_LEN 2
#define ENET_HEADER_FCS_LEN 4

#define ENET_HEADER_SIZE 14
#define ENET_FRAME_MAX_SIZE 1518
#define ENET_TYPE_OFFSET 12
#define ENET_DATA_OFFSET 14

#define ETHERTYPE_IPV4 0x0800
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IPV6 0x86DD

struct enethdr {
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
} __attribute__((packed));

#endif /* _ETHERNET_H_ */