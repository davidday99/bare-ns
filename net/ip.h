#ifndef _IP_H_
#define _IP_H_

#include <stdint.h>

#define IP_RX_BUF_LEN 10
#define IP_TX_BUF_LEN 10

struct ip_pkt {
    uint8_t dest[4];
    uint8_t src[4];
    uint8_t protocol;
    uint8_t *payload;
    uint16_t plen;
};

#endif /* _IP_H_ */