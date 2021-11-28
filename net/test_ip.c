#include "ip.h"
#include "test_ip.h"
#include <stdint.h>
#include <stdio.h>

int test_ip() {
    struct ip_pkt *p;

    uint8_t data1[] = {
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

    uint8_t data2[] = {
        0b01000110,
        0b00000000,
        0b00000000,
        0b00011010,

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

        0b11111010,
        0b10101010,
        0b11001100,
        0b10101111,

        0b11001100,
        0b00010001
    };

    write_rx_pkt(data1);
    write_rx_pkt(data2);

    while (p = read_rx_pkt())
        print_ip_pkt(p);

}

void print_ip_pkt(struct ip_pkt *p) {
    printf("IP header 1: ");
    printf("0x%X\n", p->header1);
    printf("\n\n");

    printf("IP header 2: ");
    printf("0x%X\n", p->header2);
    printf("\n\n");

    printf("IP header 3: ");
    printf("0x%X\n", p->header3);
    printf("\n\n");

    uint32_t mask;
    uint8_t shift;
    printf("Src. IP: ");
    for (uint8_t j = 0; j < 4; j++) {
        shift = 8*(3 - j);
        mask = 0xFF << shift;
        printf("%d%c", (p->src & mask) >> shift, j < 3 ? '.' : ' ');
    }
    printf("\n\n");

    printf("Dest. IP: ");
    for (uint8_t j = 0; j < 4; j++) {
        shift = 8*(3 - j);
        mask = 0xFF << shift;
        printf("%d%c", (p->dest & mask) >> shift, j < 3 ? '.' : ' ');
    }
    printf("\n\n");

    printf("IP Options length: %d\n\n", p->olen);

    printf("IP Options:\n");
    for (uint8_t j = 0; j < p->olen * 4; j++)
        printf("0x%X\n", p->options[j]);
    printf("\n\n");
    
    printf("IP Data length: %d\n\n", p->dlen);

    printf("IP Data:\n", p->dlen);
    for (uint8_t j = 0; j < p->dlen; j++)
        printf("0x%X\n", p->data[j]);
    printf("\n\n");
}