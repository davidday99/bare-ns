#ifndef _TCP_H_
#define _TCP_H_

#include <stdint.h>

struct tcphdr {
    uint16_t srcport;
    uint16_t destport;
    uint32_t seqnum;
    uint32_t acknum;
    uint16_t flags;
    uint32_t window;
    uint16_t chksm;
    uint16_t urgentptr;
};

#endif /* _TCP_H_ */