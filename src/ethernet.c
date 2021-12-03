#include "ethernet.h"
#include "ip.h"

static struct enet_frame enet_rx_buffer[ENET_RX_BUF_LEN];
static struct enet_frame enet_tx_buffer[ENET_TX_BUF_LEN];

static uint8_t rxwrptr;
static uint8_t rxrdptr;

static uint8_t txwrptr;
static uint8_t txrdptr;

void deliver_frame() {
    struct enet_frame *e = read_rx_frame();
    if (!e)
        return;

    switch (e->type) {
        case ETYPE_IP:
            write_rx_pkt(e->data);

    }
}

uint8_t write_rx_frame(struct enet_frame *e) {
    uint16_t i = 0;

    if (rxwrptr < rxrdptr)
        return 0;
    
    for (int8_t j = 0; j < ENET_DEST_LEN; i++, j++)
        rx_buffer[rxwrptr].dest[j] = e->dest[i];

    for (int8_t j = 0; j < ENET_SRC_LEN; i++, j++)
        rx_buffer[rxwrptr].src[j] = e->src[i];

    rx_buffer[rxwrptr].type = e->type;

    for (uint16_t j = 0; j < e->dlen; i++, j++)
        rx_buffer[rxwrptr].data[j] = e->data[i];

    rx_buffer[rxwrptr].dlen = e->dlen;

    for (int8_t j = 0; j < ENET_FCS_LEN; i++, j++)
        rx_buffer[rxwrptr].fcs[j] = e->fcs[i];

    rxwrptr = (rxwrptr + 1) % ENET_RX_BUF_LEN;

    return 1;
}


struct enet_frame *read_rx_frame() {
    struct enet_frame *ptr;
    if (rxrdptr == rxwrptr) {
        ptr = 0;
    } else {
        ptr = &rx_buffer[rxrdptr];
        rxrdptr = (rxrdptr + 1) % ENET_RX_BUF_LEN;
    }

    return ptr;
}

void init_frame(struct enet_frame *e, uint8_t *buf, uint16_t dlen) {
    uint16_t i = 0;
    for (int8_t j = 0; j < ENET_DEST_LEN; i++, j++)
        e->dest[j] = buf[i];

    for (int8_t j = 0; j < ENET_SRC_LEN; i++, j++)
        e->src[j] = buf[i];

    e->type = (buf[i] << 8) | buf[i + 1];
    i += ENET_TYPE_LEN;

    for (uint16_t j = 0; j < dlen; i++, j++)
        e->data[j] = buf[i];

    e->dlen = dlen;

    for (int8_t j = 0; j < ENET_FCS_LEN; i++, j++)
        e->fcs[j] = buf[i];
}