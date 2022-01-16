#ifndef _ARP_H_
#define _ARP_H_

#include <stdint.h>

#define ARP_HW_TYPE_ENET 0x0001
#define ARP_PTYPE_IPV4 0x0800

#define ARP_HW_ENET_LEN 6
#define ARP_P_IPV4_LEN 4

#define ARP_OP_REQUEST 0x0001
#define ARP_OP_REPLY 0x0002
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

void arp_send_request(uint8_t *hwsender, uint32_t psender,
                uint8_t *hwtarget, uint32_t ptarget);
void arp_send_reply(uint8_t *hwsender, uint32_t psender,
                uint8_t *hwtarget, uint32_t ptarget);
void arp_deliver(uint8_t *buf);
                    
#endif /* _ARP_H_ */