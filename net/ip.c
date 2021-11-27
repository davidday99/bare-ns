#include "ip.h"

static struct ip_pkt rx_buffer[IP_RX_BUF_LEN];
static struct ip_pkt tx_buffer[IP_TX_BUF_LEN];

static uint8_t rxrdptr;
static uint8_t rxwrptr;
static uint8_t txrdptr;
static uint8_t txwrptr;

uint8_t write_rx_pkt(struct enet_frame *e) {
    if (rxwrptr < rxrdptr)
        return 0;

    uint16_t i = 0;

    for (uint8_t j = 0; j < 4; i++, j++)
        rx_buffer[rxwrptr].dest[j] = e->data[i];

    for (uint8_t j = 0; j < 4; i++, j++)
        rx_buffer[rxwrptr].src[j] = e->data[i]; 

    // TODO: remaining IP packet fields

    rxwrptr = (rxwrptr + 1) % IP_RX_BUF_LEN;

    return 1;
}

struct ip_pkt *read_rx_pkt() {
    struct ip_pkt *ptr;
    if (rxrdptr == rxwrptr) {
        ptr = 0;
    } else {
        ptr = &rx_buffer[rxrdptr];
        rxrdptr = (rxrdptr + 1) % IP_RX_BUF_LEN;
    }
    return ptr;
}