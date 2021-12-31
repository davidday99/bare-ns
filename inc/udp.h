#ifndef _UDP_H_
#define _UDP_H_

#include <stdint.h>

struct udphdr {
    uint16_t srcport;
    uint16_t destport;
    uint16_t len;
    uint16_t cksm;
};

#define UDP_DATA_OFFSET 64

#endif /* _UDP_H_ */