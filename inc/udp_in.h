#ifndef _UDP_IN_H_
#define _UDP_IN_H_

#include <stdint.h>

void udp_deliver(uint8_t *payload, uint32_t srcip, uint32_t destip, uint16_t len);

#endif /* _UDP_IN_H_ */