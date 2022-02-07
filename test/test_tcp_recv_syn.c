#include <stdint.h>
#include "ipv4.h"
#include "tcp.h"
#include "socket.h"
#include <string.h>

static uint8_t ipv4_tcp_seg[] = {
    0x45,  // version 4, IHL 5 (20 bytes header + 0 bytes options),
    0x01,  // TOS 1 (not sure what typical values are for this field)
    0x00,
    0x28,  // Total len, 40 bytes (20 bytes header + 0 bytes options + 20 bytes data)
    0xAB,
    0XCD,  // ID, 0xABCD
    0x4F,
    0xAA,  // flags = 010, offset = 0b0111110101010
    0x40,  // TTL, 64
    0x06,  // Protocol, 0x06 (TCP)
    0xAB,
    0xCD,  // Header checksum
    0xC0,
    0xA8,
    0x00,
    0x6F,  // Src ip, 192.168.0.111
    0xC0,
    0xA8,
    0x00,
    0x01,  // Dest ip, 192168.0.1
    0xC0,
    0xC1,   // tcp payload, src port
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
    0xD2,
    0x30,  // checksum
    0x00,
    0x00,  // urgent pointer (none)
};

static uint8_t expected[] = {
    0x00,
    0x50,
    0xC0,
    0xC1,
    0x00,
    0x00,
    0x00,
    0x00,
    0xAB,
    0xCD,
    0xEF,
    0x12,
    0x60,
    0x12,
    0x04,
    0x00,
    0x78,
    0xC1,
    0x00,
    0x00,
    0x02,
    0x04,
    0x04,
    0x00
};


int test_tcp_recv_syn() {
    int success = 1;

    struct socket *s = socket_init(SOCKTYPE_TCP);
    socket_bind(s, 80);
    s->tcb.state = LISTENING;

    ipv4_deliver(ipv4_tcp_seg);
    socket_close(s);

    success &= memcmp(s->tcb.txbuf.ringbuf, expected, 20) == 0;

    return success;
}