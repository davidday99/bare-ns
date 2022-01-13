#include <stdint.h>
#include "ipv4.h"
#include "socket.h"
#include "udp.h"
#include "netcommon.h"

struct socket sockets[AVAILABLE_SOCKETS_NUM];

struct socket *socket_init(enum SOCKET_TYPE socktype) {
    for (uint8_t i = 0; i < AVAILABLE_SOCKETS_NUM; i++) {
        if (!sockets[i].open) {
            sockets[i].sockbuf.rdptr = 0;
            sockets[i].sockbuf.wrptr = 0;
            sockets[i].dest = 0;
            sockets[i].srcport = 0;
            sockets[i].socktype = socktype;
            sockets[i].open = SOCKET_OPEN;
            return &sockets[i];
        }
    }
    return NULL;
}

void socket_bind(struct socket *sock, struct socket_addr *sockaddr) {
    sock->dest = sockaddr->ip;
    sock->srcport = sockaddr->port;
}

void socket_close(struct socket *sock) {
    sock->open = SOCKET_NOT_OPEN;
}

uint16_t socket_read(struct socket *sock, struct socket_addr *sockaddr, uint8_t *buf, uint16_t len) {
    while (sock->sockbuf.rdptr == sock->sockbuf.wrptr)
        ;
    uint32_t ipv4 = *((uint32_t *) &sock->sockbuf.ringbuf[sock->sockbuf.rdptr]);
    sockaddr->ip = ipv4;
    if (sock->socktype == SOCKTYPE_UDP) {
        struct udphdr *hdr = (struct udphdr *) &sock->sockbuf
                            .ringbuf[(sock->sockbuf.rdptr + IP_METADATA_SIZE) % SOCKBUF_LEN];
        sockaddr->port = hton16(hdr->srcport);
        sock->sockbuf.rdptr = (sock->sockbuf.rdptr + UDP_HEADER_SIZE) % SOCKBUF_LEN;
    }
    /* advance the read pointer past all the header data */
    sock->sockbuf.rdptr = (sock->sockbuf.rdptr + IP_METADATA_SIZE) % SOCKBUF_LEN;

    uint16_t i = 0;
    while (i < len && sock->sockbuf.rdptr != sock->sockbuf.wrptr) {
        buf[i++] = sock->sockbuf.ringbuf[sock->sockbuf.rdptr];
        sock->sockbuf.rdptr = (sock->sockbuf.rdptr + 1) % SOCKBUF_LEN;
    }

    return i;
}

void socket_sendto(struct socket *sock, struct socket_addr *sockaddr, uint8_t *buf, uint16_t len) {
    udp_send(sock->srcport, sockaddr->ip, sockaddr->port, buf, len);
}

uint16_t socket_write_buffer(struct socket *sock, uint8_t *buf, uint16_t len) {
    uint16_t i = 0;

    while (i < len) {
        if (sock->sockbuf.wrptr == sock->sockbuf.rdptr - 1)
            return i;
        sock->sockbuf.ringbuf[sock->sockbuf.wrptr] = buf[i++];
        sock->sockbuf.wrptr = (sock->sockbuf.wrptr + 1) % SOCKBUF_LEN;
    }
    return i;
}

struct socket *socket_get_listener(struct socket_addr *sockaddr, enum SOCKET_TYPE socktype) {
    for (uint8_t i = 0; i < AVAILABLE_SOCKETS_NUM; i++) {
        if (sockets[i].open && sockets[i].srcport == sockaddr->port && sockets[i].socktype == socktype &&
            (sockets[i].dest == sockaddr->ip || sockets[i].dest == SOCKADDR_IP_ANY))
            return &sockets[i];
    }
    return NULL;
}
