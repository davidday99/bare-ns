#include <stdint.h>
#include "socket.h"
#include "ipv4.h"
#include "net.h"
#include "string.h"

extern uint8_t tx_buf[];

static uint8_t expected[] = {
    0x45,
    0x00, 
    0x00,
    0x20,
    0xFF,
    0xFF,
    0x40,
    0x00,
    0x40,
    0x11,
    0x7A,
    0x24,  // ipv4 checksum
    0x00,
    0x00,
    0x00,
    0x00,
    0xc0,
    0xa8,
    0x00,
    0x01,
    0x00,
    0x50,
    0x00,
    0x10,
    0x00,
    0x0C,
    0xA1, 
    0x2F,  // udp checksum
    0xDE,
    0xAD,
    0xBE,
    0xEF
};

int test_socket_send() {
    int success = 1;

    struct socket *s = socket_init(SOCKTYPE_UDP);
    struct socket_addr receiver_addr = {ipv4_to_int("192.168.0.1"), 16};

    socket_bind(s, 80);

    uint8_t data[4] = {0xDE, 0xAD, 0xBE, 0xEF};

    socket_sendto(s, &receiver_addr, data, 4);
    socket_close(s);

    success &= memcmp(expected, tx_buf, 32) == 0;
    
    return success;
}