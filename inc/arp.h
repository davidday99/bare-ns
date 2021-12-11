#ifndef _ARP_H_
#define _ARP_H_

#include <stdint.h>

#define ARP_OP_REQUEST 1
#define ARP_OP_REPLY 2
#define ARP_SIZE 28

struct arphdr {
    uint16_t hwtype;
    uint16_t ptype;
    uint8_t hwlen;
    uint8_t plen;
    uint16_t opcode;
    uint8_t hwsender[6];
    uint32_t psender;
    uint8_t hwtarget[6];
    uint32_t ptarget;
} __attribute__((packed));

void arp_request(uint8_t *buf, uint8_t *arppkt);
void arp_reply(uint8_t *buf, uint8_t *arppkt, uint8_t *sender_mac);

#endif /* _ARP_H_ */