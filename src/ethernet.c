#include <stdint.h>
#include "ethernet.h"
#include "arp.h"
#include "netcommon.h"

static uint8_t enet_tx_buffer[ENET_TX_BUF_LEN];
static uint16_t enet_tx_data_len;

uint8_t enet_rx_waiting;
uint8_t enet_tx_waiting;

static void enet_handle_payload(uint8_t *buf, uint16_t etype);
static void enet_deliver_ipv4();
static void enet_handle_arp();

uint16_t ethernet_handle_frame(uint8_t *rx_buf) {
    enet_rx_waiting = 0;
    uint16_t etype = hton16(((struct enethdr *) rx_buf)->type);
    enet_handle_payload(rx_buf + ENET_DATA_OFFSET, etype);
    return etype;
}

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

static void enet_handle_payload(uint8_t *buf, uint16_t etype) {
    switch (etype) {
        case ETHERTYPE_IPV4:
            enet_deliver_ipv4(buf);
            break;
        case ETHERTYPE_ARP:
            enet_handle_arp(buf);
            break;
        default:
            break;
    }
}

static void enet_deliver_ipv4(uint8_t *buf) {
    return;
}

static void enet_handle_arp(uint8_t *buf) {
    uint8_t mac[] = {0xA0, 0xCD, 0xEF, 0x12, 0x34, 0x56};
    uint8_t arpbuf[26];
    struct enethdr hdr;
    hdr.type = hton16(ETHERTYPE_ARP);
    uint16_t opcode = hton16(((struct arphdr *) buf)->opcode);
    if (opcode == ARP_OP_REQUEST)
        arp_reply(arpbuf, buf, (uint8_t []) {0xA0, 0xCD, 0xEF, 0x12, 0x34, 0x56});
        for (uint8_t i = 0; i < 6; i++) {
            hdr.src[i] = mac[i];
            hdr.dest[i] = ((struct arphdr *) arpbuf)->hwtarget[i];
        }
        ethernet_write_tx_buffer(&hdr, arpbuf, ARP_SIZE);
}