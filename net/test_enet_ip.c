#include "ethernet.h"
#include "ip.h"
#include "test_enet.h"
#include "test_ip.h"
#include <stdint.h>

int test_enet_ip() {
    uint8_t ip_pkt[] = {
        0b01000101,
        0b00000000,
        0b00000000,
        0b00010110,

        0b00000000,
        0b00000000,
        0b01000000,
        0b00000000,

        0b01000000,
        0b00000001,
        0b10101010,
        0b10101010,

        0b11000000,
        0b10101000,
        0b00000000,
        0b00000001,


        0b11000000,
        0b10101000,
        0b00000000,
        0b00110000,

        0b10101010,
        0b11111111
    };

    uint8_t enet_frame[] = {
        0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45,
        0x01, 0x23, 0x45, 0xAB, 0xCD, 0xEF,
        0x00, 0x08,
        0x01, 0x23, 0x45, 0xAB
    };

    uint8_t enet_ip_pkt[sizeof(ip_pkt) + sizeof(enet_frame)];

    int i = 0;
    for (int j = 0; j < 14; i++, j++)   
        enet_ip_pkt[i] = enet_frame[j];
    for (int j = 0; j < sizeof(ip_pkt); i++, j++)
        enet_ip_pkt[i] = ip_pkt[j];
    for (int j = 14; j < 18; i++, j++)
        enet_ip_pkt[i] = enet_frame[j];

    write_rx_frame(enet_ip_pkt, sizeof(ip_pkt));
    deliver_frame();

    struct ip_pkt *p;
    while (p = read_rx_pkt())
        print_ip_pkt(p);

    return 0;
}