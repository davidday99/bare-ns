#include "ethernet.h"
#include "test_enet.h"
#include <stdint.h>
#include <stdio.h>

int test_enet() {
    struct enet_frame *e;
    uint8_t data1[] = {
        0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45,
        0x01, 0x23, 0x45, 0xAB, 0xCD, 0xEF,
        0x00, 0x08,
        0xDE, 0xAD, 0xBE, 0xEF,
        0x01, 0x23, 0x45, 0xAB
    };

    uint8_t data2[] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
        0x00, 0x08,
        0xFE, 0xED,
        0x01, 0x23, 0x45, 0xAB
    };

    write_rx_frame(data1, 4);
    write_rx_frame(data2, 2);

    while (e = read_rx_frame())
        print_enet_frame(e);

}

void print_enet_frame(struct enet_frame *e) {
    printf("Dest. MAC: ");
    for (int i = 0; i < ENET_DEST_LEN; i++) {
        printf("%X" "%s", e->dest[i], i == 5 ? "\n\n" : "-");
    }

    printf("Src. MAC: ");
    for (int i = 0; i < ENET_SRC_LEN; i++) {
        printf("%X" "%s", e->src[i], i == 5 ? "\n\n" : "-");
    }

    printf("Type: 0x%X\n\n", e->type);

    printf("Payload size: %u\n\n", e->dlen);
    
    printf("Payload: \n");
    for (uint8_t i = 0; i < e->dlen; i++) 
        printf("0x%X\n", e->data[i]);

    printf("\n\n");

    printf("FCS: \n");
    for (uint8_t i = 0; i < ENET_FCS_LEN; i++) 
        printf("0x%X\n", e->fcs[i]);

    printf("\n\n");

}