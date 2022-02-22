#ifndef _NET_H_
#define _NET_H_

#include <stdint.h>

__attribute__ ((weak)) void net_tx(uint8_t *destmac, uint8_t *data, uint16_t len, uint16_t type);
__attribute__ ((weak)) void net_rx(uint8_t *buf);
__attribute__ ((weak)) void net_mac_address(uint8_t *buf);

#endif /* _NET_H_ */