#include <stdint.h>
#include "ip.h"
#include "arp.h"
#include "ethernet.h"

enum Layer {
    PHYSICAL = 1,
    DATALINK,
    NETWORK,
    TRANSPORT,
    SESSION,
    PRESENTATION,
    APPLICATION
};

enum Protocol {
    ETHERNET,
    ARP,
    IP,
    TCP
};

void deliver(void *data, uint16_t dlen, enum Layer dest_layer, enum Protocol p) {
    switch (dest_layer) {
        case TRANSPORT:
            // send to transport layer
            break;
        case NETWORK:
            switch (p) {
            case IP:
                write_rx_pkt((uint8_t*) data);
                break;
            case ARP:
                write_rx_arp_pkt((struct arp_pkt*) data);
                break;
            default:
                break;
            }
        case DATALINK:
            write_rx_frame((struct enet_frame*) data);
    }
}