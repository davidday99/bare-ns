#ifndef _NET_DEV_H_
#define _NET_DEV_H_

#include <stdint.h>

#define NET_DEV_RX_BUF_LEN 4000
#define NET_DEV_TX_BUF_LEN 4000

struct net_dev {
    uint8_t *rx_buf;
    uint8_t *tx_buf;
    uint8_t (*send)();
    uint8_t (*recv)();
};

void init_net_dev(struct net_dev *nd, uint8_t (*send)(), uint8_t (*recv)());
uint8_t send_data(struct net_dev *nd, uint16_t bytes);
uint8_t recv_data(struct net_dev *nd, uint16_t bytes);

#endif /* _NET_DEV_H_ */