#include "ip.h"

static struct ip_pkt ip_rx_buffer[IP_RX_BUF_LEN];
static struct ip_pkt ip_tx_buffer[IP_TX_BUF_LEN];

static uint8_t rxrdptr;
static uint8_t rxwrptr;
static uint8_t txrdptr;
static uint8_t txwrptr;

uint8_t write_rx_pkt(uint8_t *data) {
    if (rxwrptr < rxrdptr)
        return 0;

    uint16_t i = 0;

    rx_buffer[rxwrptr].header1 = (data[i] << 24) |
                                    (data[i + 1] << 16) |
                                    (data[i + 2] << 8) |
                                    (data[i + 3]);
    i += 4;
    rx_buffer[rxwrptr].header2 = (data[i] << 24) |
                                    (data[i + 1] << 16) |
                                    (data[i + 2] << 8) |
                                    (data[i + 3]);
    i += 4;
    rx_buffer[rxwrptr].header3 = (data[i] << 24) |
                                    (data[i + 1] << 16) |
                                    (data[i + 2] << 8) |
                                    (data[i + 3]);
    i += 4;
    rx_buffer[rxwrptr].src = (data[i] << 24) |
                                    (data[i + 1] << 16) |
                                    (data[i + 2] << 8) |
                                    (data[i + 3]);
    i += 4;
    rx_buffer[rxwrptr].dest = (data[i] << 24) |
                                    (data[i + 1] << 16) |
                                    (data[i + 2] << 8) |
                                    (data[i + 3]);
    i+= 4;

    uint8_t olen = (rx_buffer[rxwrptr].header1 & 0xF000000) >> 24;
    rx_buffer[rxwrptr].options = &data[i];
    rx_buffer[rxwrptr].olen = olen - 5;
    i += (olen - 5)*4;

    uint16_t dlen = rx_buffer[rxwrptr].header1 & 0xFF;
    dlen -= olen * 4;
    rx_buffer[rxwrptr].data = &data[i];
    rx_buffer[rxwrptr].dlen = dlen;

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