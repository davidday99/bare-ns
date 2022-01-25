#include "checksum.h"
#include "tcp.h"

uint8_t tcp_pseudohdr[] = {
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

uint8_t tcp_seg[] = {
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



int test_tcp_checksum() {
    int success = 1;

    struct tcphdr *hdr = (struct tcphdr *) tcp_seg;

    hdr->cksm = calculate_tcp_checksum(tcp_seg, tcp_pseudohdr);

    uint16_t verify_checksum = calculate_tcp_checksum(tcp_seg, tcp_pseudohdr);

    success &= hdr->cksm == 0x6126;
    success &= verify_checksum == 0;
    return success;
}