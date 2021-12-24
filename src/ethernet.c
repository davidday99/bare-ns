#include <stdint.h>
#include "ethernet.h"
#include "arp.h"
#include "netcommon.h"

static uint8_t enet_tx_buffer[ENET_TX_BUF_LEN];
static uint16_t enet_tx_data_len;

uint8_t enet_rx_waiting;
uint8_t enet_tx_waiting;

void ethernet_write_tx_buffer(struct enethdr *hdr, uint8_t *data, uint16_t len) {
    uint16_t i = 0;
    for (; i < ENET_HEADER_SIZE; i++)
        enet_tx_buffer[i] = ((uint8_t *) hdr)[i];
    for (uint16_t j = 0; j < len; i++, j++)
            enet_tx_buffer[i] = data[j];
    
    enet_tx_data_len = len + ENET_HEADER_SIZE;
    enet_tx_waiting = 1;
}

void ethernet_read_tx_buffer(uint8_t *buf) {
    for (uint16_t i = 0; i < enet_tx_data_len; i++)
        buf[i] = enet_tx_buffer[i];

    enet_tx_data_len = 0;
    enet_tx_waiting = 0;
}