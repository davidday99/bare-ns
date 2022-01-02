#include "socket.h"
#include "ipv4.h"
#include "udp.h"
#include <string.h>


/* This function internal, not part of the API, and thus not listed in the header file. */
uint16_t socket_write_buffer(struct socket *sock, uint8_t *buf, uint16_t len);

uint8_t socket_udp_data[] = {
    0xEF,
    0xBE,
    0xAD,
    0xDE,  // src ip
    0xAB,
    0xCD,
    0xEF,
    0x12,  // dest ip
    0xC0,
    0xC1,  // src port
    0xAF,
    0xFA,  // dest port
    0x00,
    0x0C,  // size of UDP header plus data (2 bytes of data)
    0xAB,
    0xCD,  // dummy checksum
    0xFF,
    0xFF
};


int test_socket_init() {
    int success = 1;
    struct socket *s = socket_init(SOCKTYPE_UDP);
    success &= s->dest == 0;
    success &= s->srcport == 0;
    success &= s->socktype == SOCKTYPE_UDP;
    success &= s->open == SOCKET_OPEN;

    socket_close(s);

    return success;
}

int test_socket_bind() {
    int success = 1;
    struct socket *s = socket_init(SOCKTYPE_UDP);
    struct socket_addr saddr = {ipv4_to_int("192.168.0.1"), 42};
    socket_bind(s, &saddr);
    success &= s->dest == 0xC0A80001;
    success &= s->srcport == 42;
    
    socket_close(s);

    return success;
}

int test_socket_close() {
    int success = 1;
    struct socket *s = socket_init(SOCKTYPE_UDP);
    s->dest = 0xFFFFFFFF;
    s->srcport = 42;
    socket_close(s);
    success &= s->open == SOCKET_NOT_OPEN;
    
    return success;
}

int test_socket_get_listener() {
    int success = 1;
    struct socket *s = socket_init(SOCKTYPE_UDP);
    struct socket_addr saddr = {ipv4_to_int("192.168.0.1"), 42};
    socket_bind(s, &saddr);
    struct socket *listener = socket_get_listener(&saddr, SOCKTYPE_UDP);
    success &= s == listener;
    socket_close(s);
    return success;
}

int test_socket_read_udp() {
    int success = 1;

    struct socket *s = socket_init(SOCKTYPE_UDP);
    memcpy(s->sockbuf.ringbuf, socket_udp_data, sizeof(socket_udp_data));
    s->sockbuf.wrptr = sizeof(socket_udp_data);
    uint8_t data[sizeof(socket_udp_data) - UDP_HEADER_SIZE - IP_METADATA_SIZE];  // subtract UDP and IP headers
    struct socket_addr sockaddr;
    socket_read(s, &sockaddr, data, 2);

    success &= memcmp(data, (uint8_t[]) {0xFF, 0xFF}, 2) == 0;
    success &= sockaddr.ip == 0xDEADBEEF;
    success &= sockaddr.port == 0xC0C1;
    success &= s->sockbuf.rdptr == s->sockbuf.wrptr;
    socket_close(s);
    return success;
}

int test_socket_write_buffer() {
    int success = 1;
    struct socket *s = socket_init(SOCKTYPE_UDP);
    socket_write_buffer(s, socket_udp_data, sizeof(socket_udp_data));
    success &= memcmp(s->sockbuf.ringbuf, socket_udp_data, sizeof(socket_udp_data)) == 0;
    socket_close(s);
    return success;
}

int test_socket() {
    int success = 1;
    success &= test_socket_init();
    success &= test_socket_bind();
    success &= test_socket_close();
    success &= test_socket_get_listener();
    success &= test_socket_read_udp();
    success &= test_socket_write_buffer();
    return success;
}