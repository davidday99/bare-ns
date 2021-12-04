#ifndef _DELIVER_H_
#define _DELIVER_H_

#include <stdint.h>
#include "ip.h"
#include "arp.h"
#include "ethernet.h"

uint8_t deliver_frame(struct enet_frame *e);
uint8_t deliver_ip_enet(struct ip_pkt *p);
uint8_t deliver_arp_enet(struct arp_pkt *a);

#endif /* _DELIVER_H_ */