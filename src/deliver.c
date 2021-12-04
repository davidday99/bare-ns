#include <stdint.h>
#include "deliver.h"

static uint8_t deliver_enet_ip(struct enet_frame *e);
static uint8_t deliver_enet_arp(struct enet_frame *e);

uint8_t deliver_frame(struct enet_frame *e) {
    if (!e)
        return 0;

    uint8_t stat;
    switch (e->type) {
        case ETYPE_IP:
            stat = deliver_enet_ip(e);
            break;
        case ETYPE_ARP:
            stat = deliver_enet_arp(e);
            break;
        default:
            stat = 0;
            break;
    }
    return stat;
}

static uint8_t deliver_enet_ip(struct enet_frame *e) {
    struct ip_pkt p;
    init_ip_pkt(&p, e->data);
    return write_rx_ip_pkt(&p);
}

static uint8_t deliver_enet_arp(struct enet_frame *e) {
    struct arp_pkt a;
    init_arp_pkt(&a, e->data);
    return write_rx_arp_pkt(&a);
}

uint8_t deliver_ip_enet(struct ip_pkt *p) {
    struct enet_frame e;

    uint16_t i = 0;

    e.type = ETYPE_IP;
    
    for (int8_t j = 3; j >= 0; j--)
        e.data[i++] = *((uint8_t *) &p->header1 + j); 

    for (int8_t j = 3; j >= 0; j--) 
        e.data[i++] = *((uint8_t *) &p->header2 + j); 

    for (int8_t j = 3; j >= 0; j--) 
        e.data[i++] = *((uint8_t *) &p->header3 + j); 

    for (int8_t j = 3; j >= 0; j--) 
        e.data[i++] = *((uint8_t *) &p->src + j); 

    for (int8_t j = 3; j >= 0; j--)
        e.data[i++] = *((uint8_t *) &p->dest + j); 

    for (uint8_t j = 0; j < p->olen; j++)
        e.data[i++] = p->options[j];

    for (uint8_t j = 0; j < p->dlen; j++) 
        e.data[i++] = p->data[j];
    
    return write_tx_frame(&e);
}

uint8_t deliver_arp_enet(struct arp_pkt *a) {
    struct enet_frame e;

    uint16_t i = 0; 

    e.type = ETYPE_ARP;
    
    for (int8_t j = 1; j >= 0; j--)
        e.data[i++] = *((uint8_t *) &a->hw_type + j); 

    for (int8_t j = 1; j >= 0; j--)
        e.data[i++] = *((uint8_t *) &a->protocol_type + j); 

    e.data[i++] = a->hwlen;
    e.data[i++] = a->plen;
    
    for (int8_t j = 1; j >= 0; j--)
        e.data[i++] = *((uint8_t *) &a->opcode + j); 

    for (uint8_t j = 0; j < a->hwlen; j++)
        e.data[i++] = a->hwaddr_sender[j];

    for (uint8_t j = 0; j < a->plen; j++)
        e.data[i++] = a->paddr_sender[j];

    for (uint8_t j = 0; j < a->hwlen; j++)
        e.data[i++] = a->hwaddr_target[j];

    for (uint8_t j = 0; j < a->plen; j++)
        e.data[i++] = a->paddr_target[j];

    return write_tx_frame(&e);
}






