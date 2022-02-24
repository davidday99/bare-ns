#ifndef _NET_H_
#define _NET_H_

#include <stdint.h>

extern const uint8_t MAC_BROADCAST[];
extern uint8_t GATEWAY_MAC[];

__attribute__ ((weak)) void net_tx(const uint8_t *destmac, uint8_t *data, uint16_t len, uint16_t type);
__attribute__ ((weak)) void net_rx(uint8_t *buf);
__attribute__ ((weak)) void net_mac_address(uint8_t *buf);

#endif /* _NET_H_ */