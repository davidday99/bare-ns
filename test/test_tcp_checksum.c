#include "checksum.h"
#include "tcp.h"

static uint8_t tcp_pseudohdr1[] = {
    0xC0,
    0xA8,
    0x00,
    0x6F,  // Src ip, 192.168.0.111
    0xC0,
    0xA8,
    0x00,
    0x01,  // Dest ip, 192168.0.1
    0x00,  // zero pad
    0x06,
    0x00,
    0x16  // size
};

static uint8_t tcp_pseudohdr2[] = {
    0xC0,
    0xA8,
    0x00,
    0x6F,  // Src ip, 192.168.0.111
    0xC0,
    0xA8,
    0x00,
    0x01,  // Dest ip, 192168.0.1
    0x00,  // zero pad
    0x06,
    0x00,
    0x15  // size
};

static uint8_t tcp_pseudohdr3[] = {
    0xC0,
    0xA8,
    0x00,
    0x6F,  // Src ip, 192.168.0.111
    0xC0,
    0xA8,
    0x00,
    0x81,  // Dest ip, 192168.0.129
    0x00,  // zero pad
    0x06,
    0x00,
    0x36  // size
};

static uint8_t tcp_seg1[] = {
    0xC0,
    0xC1,  // src port
    0x00,
    0x50,  // dest port
    0xAB,
    0xCD,
    0xEF,
    0x11,  // ISN
    0x00,
    0x00,
    0x00,
    0x00,  // initial ACK, not yet set
    0x50,  // offset of 5 (high nibble) plus reserved (low nibble)
    0x02,  // SYN flag
    0xFF,
    0xFF,  // window
    0x00,
    0x00,  // zero for checksum
    0x00,
    0x00,  // urgent pointer (none)
    0xAB,  
    0xCD  // 2 bytes of data
};

static uint8_t tcp_seg2[] = {
    0xC0,
    0xC1,  // src port
    0x00,
    0x50,  // dest port
    0xAB,
    0xCD,
    0xEF,
    0x11,  // ISN
    0x00,
    0x00,
    0x00,
    0x00,  // initial ACK, not yet set
    0x50,  // offset of 5 (high nibble) plus reserved (low nibble)
    0x02,  // SYN flag
    0xFF,
    0xFF,  // window
    0x00,
    0x00,  // zero for checksum
    0x00,
    0x00,  // urgent pointer (none)
    0xAB,  // 1 byte of data
};

static uint8_t tcp_seg3[] = {
    0x1F,
    0x40,
    0x9E,
    0x22,
    0x00,
    0x00,
    0x00,
    0x01,
    0x39,
    0xC3,
    0x5D,
    0xC9,
    0x50,
    0x18,
    0x04,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x48,
    0x65,
    0x6c,
    0x6c,
    0x6f,
    0x20,
    0x54,
    0x43,
    0x50,
    0x20,
    0x53,
    0x65,
    0x72,
    0x76,
    0x65,
    0x72
};



int test_tcp_checksum() {
    int success = 1;

    struct tcphdr *hdr1 = (struct tcphdr *) tcp_seg1;
    struct tcphdr *hdr2 = (struct tcphdr *) tcp_seg2;
    struct tcphdr *hdr3 = (struct tcphdr *) tcp_seg3;

    hdr1->cksm = calculate_tcp_checksum(tcp_seg1, tcp_pseudohdr1);

    uint16_t verify_checksum1 = calculate_tcp_checksum(tcp_seg1, tcp_pseudohdr1);

    hdr2->cksm = calculate_tcp_checksum(tcp_seg2, tcp_pseudohdr2);

    uint16_t verify_checksum2 = calculate_tcp_checksum(tcp_seg2, tcp_pseudohdr2);

    hdr3->cksm = calculate_tcp_checksum(tcp_seg3, tcp_pseudohdr3);

    uint16_t verify_checksum3 = calculate_tcp_checksum(tcp_seg3, tcp_pseudohdr3);

    success &= verify_checksum1 == 0;
    success &= verify_checksum2 == 0;
    success &= verify_checksum3 == 0;
    return success;
}