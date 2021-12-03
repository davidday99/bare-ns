#include <stdint.h>
#include "arp.h"

static struct arp_pkt arp_rx_buffer[ARP_RX_BUF_LEN];
static struct arp_pkt arp_tx_buffer[ARP_TX_BUF_LEN];

static uint8_t rxwrptr;
static uint8_t rxrdptr;

static uint8_t txwrptr;
static uint8_t txrdptr;


