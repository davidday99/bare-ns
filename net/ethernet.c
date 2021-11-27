#include "ethernet.h"

#define RX_BUF_LEN 10
#define TX_BUF_LEN 10

static struct enet_frame rx_buffer[ENET_RX_BUF_LEN];
static struct enet_frame tx_buffer[ENET_TX_BUF_LEN];

static uint8_t rxwrptr;
static uint8_t rxrdptr;

static uint8_t txwrptr;
static uint8_t txrdptr;

uint8_t write_rx_frame(uint8_t *buf, uint16_t dlen) {
    uint16_t i = 0;

    if (rxwrptr < rxrdptr)
        return 0;
    
    for (int8_t j = 5; j >= 0; i++, j--)
        rx_buffer[rxwrptr].dest[j] = buf[i];

    for (int8_t j = 5; j >= 0; i++, j--)
        rx_buffer[rxwrptr].src[j] = buf[i];

    rx_buffer[rxwrptr].type = buf[i] | (buf[i + 1] << 8);
    i += 2;

    for (uint16_t j = 0; j < dlen; i++, j++)
        rx_buffer[rxwrptr].data[j] = buf[i];

    rx_buffer[rxwrptr].dlen = dlen;

    for (int8_t j = 3; j >= 0; i++, j--)
        rx_buffer[rxwrptr].fcs[j] = buf[i];

    rxwrptr = (rxwrptr + 1) % RX_BUF_LEN;

    return 1;
}


struct enet_frame *read_rx_frame() {
    struct enet_frame *ptr;
    if (rxrdptr == rxwrptr)
        ptr = 0;
    else
        ptr = &rx_buffer[rxrdptr];
        rxrdptr = (rxrdptr + 1) % RX_BUF_LEN;

    return ptr;
}
