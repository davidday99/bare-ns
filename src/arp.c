#include <stdint.h>
#include "arp.h"
#include "ethernet.h"

static struct arp_pkt arp_rx_buffer[ARP_RX_BUF_LEN];
static struct arp_pkt arp_tx_buffer[ARP_TX_BUF_LEN];

static uint8_t rxwrptr;
static uint8_t rxrdptr;

static uint8_t txwrptr;
static uint8_t txrdptr;


void init_arp_pkt(struct arp_pkt *a, uint8_t *data) {
    uint16_t i = 0;
    a->hw_type = (data[i] << 8) | data[i + 1];
    i += 2;
    a->protocol_type = (data[i] << 8) | data[i + 1];
    i += 2;
    a->hwlen = data[i++];
    a->plen = data[i++];
    a->opcode = (data[i] << 8) | data[i + 1];
    i += 2;
    a->hwaddr_sender = &data[i++];
    a->paddr_sender = &data[i++];
    a->hwaddr_target = &data[i++];
    a->paddr_target = &data[i++];
}

uint8_t write_rx_arp_pkt(struct arp_pkt *a) {
    uint16_t i = 0;

    if (rxwrptr < rxrdptr)
        return 0;

    arp_rx_buffer[rxwrptr].hw_type = a->hw_type;
    arp_rx_buffer[rxwrptr].protocol_type = a->protocol_type;
    arp_rx_buffer[rxwrptr].hwlen = a->hwlen;
    arp_rx_buffer[rxwrptr].plen = a->plen;
    arp_rx_buffer[rxwrptr].opcode = a->opcode;
    arp_rx_buffer[rxwrptr].hwaddr_sender = a->hwaddr_sender;
    arp_rx_buffer[rxwrptr].paddr_sender = a->paddr_sender;
    arp_rx_buffer[rxwrptr].hwaddr_target = a->hwaddr_target;
    arp_rx_buffer[rxwrptr].paddr_target = a->paddr_target;

    rxwrptr = (rxwrptr + 1) % ARP_RX_BUF_LEN;
    return 1;
}    uint8_t prev = 0;

uint8_t write_tx_arp_pkt(struct arp_pkt *a) {
    uint16_t i = 0;

    if (txwrptr < txrdptr)
        return 0;

    arp_tx_buffer[txwrptr].hw_type = a->hw_type;
    arp_tx_buffer[txwrptr].protocol_type = a->protocol_type;
    arp_tx_buffer[txwrptr].hwlen = a->hwlen;
    arp_tx_buffer[txwrptr].plen = a->plen;
    arp_tx_buffer[txwrptr].opcode = a->opcode;
    arp_tx_buffer[txwrptr].hwaddr_sender = a->hwaddr_sender;
    arp_tx_buffer[txwrptr].paddr_sender = a->paddr_sender;
    arp_tx_buffer[txwrptr].hwaddr_target = a->hwaddr_target;
    arp_tx_buffer[txwrptr].paddr_target = a->paddr_target;

    txwrptr = (txwrptr + 1) % ARP_TX_BUF_LEN;
    return 1;
}

struct arp_pkt *read_rx_arp_pkt() {
    struct arp_pkt *ptr;
    if (rxrdptr == rxwrptr) {
        ptr = 0;
    } else {
        ptr = &arp_rx_buffer[rxrdptr];
        rxrdptr = (rxrdptr + 1) % ARP_RX_BUF_LEN;
    }

    return ptr;
}

struct arp_pkt *read_tx_arp_pkt() {
    struct arp_pkt *ptr;
    if (txrdptr == txwrptr) {
        ptr = 0;
    } else {
        ptr = &arp_tx_buffer[txrdptr];
        txrdptr = (txrdptr + 1) % ARP_TX_BUF_LEN;
    }

    return ptr;
}
