#include <stdint.h>
#include "socket.h"
#include "ipv4.h"
#include "net.h"
#include "string.h"

static uint8_t tx_buf[32];

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
    0x00,
    0x00,
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
    0x00,
    0x00,
    0xDE,
    0xAD,
    0xBE,
    0xEF
};

void net_tx(uint8_t *data, uint16_t len) {
    memcpy(tx_buf, data, len);
}

int test_socket_send() {
    int success = 1;

    struct socket *s = socket_init(SOCKTYPE_UDP);
    struct socket_addr server_addr = {SOCKADDR_IP_ANY, 80};
    struct socket_addr receiver_addr = {ipv4_to_int("192.168.0.1"), 16};

    socket_bind(s, &server_addr);

    uint8_t data[4] = {0xDE, 0xAD, 0xBE, 0xEF};

    socket_sendto(s, &receiver_addr, data, 4);

    success &= memcmp(expected, tx_buf, 32) == 0;
    
    return success;
}