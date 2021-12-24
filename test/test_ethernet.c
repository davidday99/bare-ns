#include "ethernet.h"
#include "netcommon.h"
#include "string.h"

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
    0x56,  // src MAC, 0xABCDEF123456
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

int test_read_enet_dest() {
    struct enethdr *hdr = (struct enethdr *) enet_frame;
    uint8_t expected[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    return memcmp(hdr->dest, expected, 6) == 0;
}

int test_read_enet_src() {
    struct enethdr *hdr = (struct enethdr *) enet_frame;
    uint8_t expected[] = {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56};
    return memcmp(hdr->src, expected, 6) == 0;
}

int test_read_enet_type() {
    struct enethdr *hdr = (struct enethdr *) enet_frame;
    uint16_t expected = 0x0806;
    return hton16(hdr->type) == expected;
}

int test_ethernet() {
    int success = 1;
    success &= test_networking_macros();
    success &= test_read_enet_dest();
    success &= test_read_enet_src();
    success &= test_read_enet_type();

    return success;
}