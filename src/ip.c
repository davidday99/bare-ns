#include "ip.h"

static struct ip_pkt ip_rx_buffer[IP_RX_BUF_LEN];
static struct ip_pkt ip_tx_buffer[IP_TX_BUF_LEN];

static uint8_t rxrdptr;
static uint8_t rxwrptr;
static uint8_t txrdptr;
static uint8_t txwrptr;

uint8_t write_rx_ip_pkt(struct ip_pkt *p) {
    if (rxwrptr < rxrdptr)
        return 0;

    ip_rx_buffer[rxwrptr].header1 = p->header1;
    ip_rx_buffer[rxwrptr].header2 = p->header2;
    ip_rx_buffer[rxwrptr].header3 = p->header3;
    ip_rx_buffer[rxwrptr].src = p->src;
    ip_rx_buffer[rxwrptr].dest = p->dest;

    ip_rx_buffer[rxwrptr].options = p->options;
    ip_rx_buffer[rxwrptr].olen = p->olen;

    ip_rx_buffer[rxwrptr].data = p->data;
    ip_rx_buffer[rxwrptr].dlen = p->dlen;

    rxwrptr = (rxwrptr + 1) % IP_RX_BUF_LEN;

    return 1;
}

struct ip_pkt *read_rx_ip_pkt() {
    struct ip_pkt *ptr;
    if (rxrdptr == rxwrptr) {
        ptr = 0;
    } else {
        ptr = &ip_rx_buffer[rxrdptr];
        rxrdptr = (rxrdptr + 1) % IP_RX_BUF_LEN;
    }
    return ptr;
}

void init_ip_pkt(struct ip_pkt *p, uint8_t *buf) {
    uint16_t i = 0;

    ip_rx_buffer[rxwrptr].header1 = (buf[i] << 24) |
                                    (buf[i + 1] << 16) |
                                    (buf[i + 2] << 8) |
                                    (buf[i + 3]);
    i += 4;
    ip_rx_buffer[rxwrptr].header2 = (buf[i] << 24) |
                                    (buf[i + 1] << 16) |
                                    (buf[i + 2] << 8) |
                                    (buf[i + 3]);
    i += 4;
    ip_rx_buffer[rxwrptr].header3 = (buf[i] << 24) |
                                    (buf[i + 1] << 16) |
                                    (buf[i + 2] << 8) |
                                    (buf[i + 3]);
    i += 4;
    ip_rx_buffer[rxwrptr].src = (buf[i] << 24) |
                                    (buf[i + 1] << 16) |
                                    (buf[i + 2] << 8) |
                                    (buf[i + 3]);
    i += 4;
    ip_rx_buffer[rxwrptr].dest = (buf[i] << 24) |
                                    (buf[i + 1] << 16) |
                                    (buf[i + 2] << 8) |
                                    (buf[i + 3]);
    i+= 4;

    uint8_t olen = (ip_rx_buffer[rxwrptr].header1 & 0xF000000) >> 24;
    ip_rx_buffer[rxwrptr].options = &buf[i];
    ip_rx_buffer[rxwrptr].olen = olen - 5;
    i += (olen - 5)*4;

    uint16_t dlen = ip_rx_buffer[rxwrptr].header1 & 0xFF;
    dlen -= olen * 4;
    ip_rx_buffer[rxwrptr].data = &buf[i];
    ip_rx_buffer[rxwrptr].dlen = dlen;
}