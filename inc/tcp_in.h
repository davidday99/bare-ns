#ifndef _TCP_IN_H_
#define _TCP_IN_H_

#include <stdint.h>

void tcp_deliver(uint8_t *payload, uint32_t srcip, uint32_t destip, uint16_t len);

#endif /* _TCP_IN_H_ */