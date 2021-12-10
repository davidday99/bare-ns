#include <stdint.h>
#include "net_dev.h"

#define NET_DEV_RX_BUF_LEN 4000
#define NET_DEV_TX_BUF_LEN 4000

static uint8_t net_dev_rx_buffer[NET_DEV_RX_BUF_LEN];
static uint8_t net_dev_tx_buffer[NET_DEV_TX_BUF_LEN];

void init_net_dev(struct net_dev *nd, uint8_t (*send)(), uint8_t (*recv)()) {
    nd->rx_buf = net_dev_rx_buffer;
    nd->tx_buf = net_dev_tx_buffer;
    nd->send = send;
    nd->recv = recv;
}

uint8_t send_data(struct net_dev *nd, uint16_t bytes) {
    nd->send(nd->tx_buf, bytes);
}

uint8_t recv_data(struct net_dev *nd, uint16_t bytes) {
    nd->recv(nd->rx_buf, bytes);
}