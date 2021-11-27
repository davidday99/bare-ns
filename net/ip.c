#include "ip.h"S

static struct ip_pkt rx_buffer[IP_RX_BUF_LEN];
static struct ip_pkt tx_buffer[IP_TX_BUF_LEN];