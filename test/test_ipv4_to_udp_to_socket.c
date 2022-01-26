#include <stdint.h>
#include "ipv4.h"
#include "udp.h"
#include "socket.h"
#include <string.h>

uint8_t ipv4_packet[] = {
    0x45,  // version 4, IHL 5 (20 bytes header + 0 bytes options),
    0x01,  // TOS 1 (not sure what typical values are for this field)
    0x00,
    0x1E,  // Total len, 30 bytes (20 bytes header + 0 bytes options + 10 bytes data)
    0xAB,
    0XCD,  // ID, 0xABCD
    0x4F,
    0xAA,  // flags = 010, offset = 0b0111110101010
    0x40,  // TTL, 64
    0x11,  // Protocol, 0x11 (UDP)
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
    0xC1,   // udp payload, src port
    0x00,
    0x50,  // dest port
    0x00,
    0x0A,  // udp header plus data length
    0xAB,
    0xCD,  // dummy checksum
    0xBE,
    0xEF  // 2 bytes of data
};


int test_ipv4_to_udp_to_socket() {
    int success;

    struct socket *s = socket_init(SOCKTYPE_UDP);
    struct socket_addr sockaddr = {ipv4_to_int("192.168.0.111"), 80};
    socket_bind(s, 80);

    ipv4_deliver(ipv4_packet);

    uint8_t data[2];
    struct socket_addr clientaddr;
    socket_recv(s, &clientaddr, data, 2);

    success &= clientaddr.ip == 0xC0A8006F;
    success &= clientaddr.port == 0xC0C1;
    success &= memcmp(data, (uint8_t[]) {0xBE, 0xEF}, 2) == 0;

    return success;
}