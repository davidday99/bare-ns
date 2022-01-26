#include <stdint.h>
#include "tcp.h"

static uint8_t tcp_seg[] = {
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
    0x02,  // MSS option  
    0x04,  // total option len = 4 bytes
    0xAB,
    0xCD  // MSS = 0xABCD
};


int test_tcp_parse_options() {
    int success = 1;
    struct TCB tcb;
    tcp_parse_options(&tcb, tcp_seg);
    return success;
}