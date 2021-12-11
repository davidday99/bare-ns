#include <stdint.h>
#include "stub_enc.h"
#include "ethernet.h"
#include "arp.h"

static uint8_t enet_frame_arppkt[] = {
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,  // dest MAC, 0xFFFFFFFF (broadcast)
    0xAB,
    0xCD,
    0xEF,
    0x12,
    0x34,
    0x56,  // src MAC, 0xABCDEF12
    0x08,
    0x06,  // type 0x0806 (ARP)
    
    // arp packet
    0x00,  
    0x01,  // hw type = ethernet
    0x08,  
    0x00,  // protocol type = IPV4
    0x06,  // hw len = 6
    0x04,  // protocol len = 4
    0x00,  
    0x01,  // opcode = 1, request
    0xFF,
    0xFF,
    0xFF,
    0xFF,  // sender IP, 0xFFFFFFFF
    0xCC,  
    0xCC,
    0xCC,
    0xCC,
    0xCC,
    0xCC,  // sender MAC, 0xCCCCCCCCCCCC
    0xAA,
    0xAA,
    0xAA,
    0xAA,  // target IP, 0xAAAAAAAA
    0,
    0,
    0,
    0,  // target MAC will be unknown, reason for send ARP request

    0x12,
    0x34,
    0x56,
    0x78  // FCS, 0x12345678
};

int test_receive_arp_pkt() {
    int pktcnt = 1;

    if (pktcnt > 0) {
        stub_enc_read(enet_frame_arppkt, sizeof(enet_frame_arppkt));
        enet_rx_waiting = 1;
    }

    if (enet_rx_waiting) {
        ethernet_handle_frame(stub_enc_rx_buffer);
    }
    
    return 1;
}

int test_ethernet_arp() {
    int success = 1;
    success &= test_receive_arp_pkt();
    return success;
}