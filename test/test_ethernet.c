#include "ethernet.h"
#include "stub_enc.h"
#include "netcommon.h"

static uint8_t enet_frame[] = {
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
    0xCC,
    0xCC,  // data, 0xCCCC
    0x12,
    0x34,
    0x56,
    0x78  // FCS, 0x12345678
};

int test_networking_macros() {
    int success = 1;
    success &= hton16(0xCDAB) == 0xABCD;
    success &= hton32(0x12EFCDAB) == 0xABCDEF12;
    return success;
}

int test_receive_ethernet_frame() {
    stub_enc_read(enet_frame, sizeof(enet_frame));
    enet_rx_waiting = 1;
    uint16_t res = 0;
    return res == ETHERTYPE_ARP;
}

int test_write_ethernet_frame_to_tx_buffer() {
    uint8_t data[14] = {
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
    };

    struct enethdr hdr;

    uint8_t res[sizeof(data) + sizeof(hdr)];

    ethernet_write_tx_buffer(&hdr, data, sizeof(data));
    ethernet_read_tx_buffer(res);

    for (int i = 0; i < sizeof(data); i++)
        if (res[i + sizeof(hdr)] != data[i])
            return 0;
    return 1;
}

int test_enc_to_ethernet() {
    int pktcnt = 1;

    if (pktcnt > 0) {
        stub_enc_read(enet_frame, sizeof(enet_frame));
        enet_rx_waiting = 1;
    }
    return 1;
}

int test_ethernet() {
    int success = 1;
    success &= test_networking_macros();
    success &= test_receive_ethernet_frame();
    success &= test_write_ethernet_frame_to_tx_buffer();
    success &= test_enc_to_ethernet();
    return success;
}