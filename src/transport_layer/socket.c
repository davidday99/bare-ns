#include <stdint.h>
#include "ipv4.h"
#include "socket.h"
#include "udp.h"
#include "netcommon.h"
#include "tcp.h"

#pragma GCC optimize ("O0")

struct socket sockets[AVAILABLE_SOCKETS_NUM];

static void socket_close_tcp(struct socket *sock);
static struct socket *socket_get_udp_listener(struct socket_addr *sockaddr);
static struct socket *socket_get_tcp_listener(struct socket_addr *sockaddr);

struct socket *socket_init(enum SOCKET_TYPE socktype) {
    for (uint8_t i = 0; i < AVAILABLE_SOCKETS_NUM; i++) {
        if (!sockets[i].open) {
            sockets[i].sockbuf.rdptr = 0;
            sockets[i].sockbuf.wrptr = 0;
            sockets[i].srcport = 0;
            sockets[i].socktype = socktype;
            sockets[i].open = SOCKET_OPEN;
            sockets[i].tcb.state = CLOSED;
            sockets[i].tcb.txbuf.rdptr = 0;
            sockets[i].tcb.txbuf.wrptr = 0;
            return &sockets[i];
        }
    }
    return NULL;
}

void socket_bind(struct socket *sock, uint16_t port) {
    sock->srcport = port;
    sock->tcb.srcport = port;
}

void socket_connect(struct socket *sock) {
    return;  // TODO: implement
}

uint8_t socket_accept(struct socket *sock) {
    sock->tcb.state = LISTENING;
    while (sock->tcb.state != ESTABLISHED) {
        if (sock->tcb.state == CLOSED)
            return 0;
    }
    return 1;
}

void socket_close(struct socket *sock) {

    if (sock->socktype == SOCKTYPE_UDP) {
        sock->open = SOCKET_NOT_OPEN;
    } else if (sock->socktype == SOCKTYPE_TCP) {
        socket_close_tcp(sock);
    }
}

static void socket_close_tcp(struct socket *sock) {
    sock->tcb.prevstate = sock->tcb.state;
    if (sock->tcb.state == ESTABLISHED) {
        sock->tcb.state = FIN_WAIT_1;
    } else if (sock->tcb.state == CLOSE_WAIT) {
        sock->tcb.state = LAST_ACK;
    }
    tcp_send_fin(&sock->tcb, sock->clientaddr.ip, sock->clientaddr.port);
}

uint16_t socket_recv(struct socket *sock, struct socket_addr *sockaddr, uint8_t *buf, uint16_t len) {
    while (sock->sockbuf.rdptr == sock->sockbuf.wrptr)
        ;
    uint32_t ipv4 = (uint32_t) (sock->sockbuf.ringbuf[sock->sockbuf.rdptr] << 24 |
                                    sock->sockbuf.ringbuf[(sock->sockbuf.rdptr + 1) % SOCKBUF_LEN] << 16 |
                                    sock->sockbuf.ringbuf[(sock->sockbuf.rdptr + 2) % SOCKBUF_LEN] << 8 |
                                    sock->sockbuf.ringbuf[(sock->sockbuf.rdptr + 3) % SOCKBUF_LEN]);
    sockaddr->ip = hton32(ipv4);
    sock->sockbuf.rdptr = (sock->sockbuf.rdptr + IP_METADATA_SIZE) % SOCKBUF_LEN;
    if (sock->socktype == SOCKTYPE_UDP) {
        uint16_t port = sock->sockbuf.ringbuf[sock->sockbuf.rdptr] << 8 |
                                    sock->sockbuf.ringbuf[(sock->sockbuf.rdptr + 1) % SOCKBUF_LEN];
        sockaddr->port = port;
        sock->sockbuf.rdptr = (sock->sockbuf.rdptr + UDP_HEADER_SIZE) % SOCKBUF_LEN;
    }
    /* advance the read pointer past all the header data */
    // sock->sockbuf.rdptr = (sock->sockbuf.rdptr + IP_METADATA_SIZE) % SOCKBUF_LEN;

    uint16_t i = 0;
    while (i < len && sock->sockbuf.rdptr != sock->sockbuf.wrptr) {
        buf[i++] = sock->sockbuf.ringbuf[sock->sockbuf.rdptr];
        sock->sockbuf.rdptr = (sock->sockbuf.rdptr + 1) % SOCKBUF_LEN;
    }

    return i;
}

uint16_t socket_read(struct socket *sock, uint8_t *buf, uint16_t len) {
    while (sock->sockbuf.rdptr == sock->sockbuf.wrptr) {
        if (sock->tcb.state == CLOSED) {
            return 0;
        }
    }
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

uint16_t socket_send(struct socket *sock, uint8_t *buf, uint16_t len) {
    if (sock->socktype != SOCKTYPE_TCP || sock->tcb.state != ESTABLISHED)
        return 0;
    tcp_transmit_message(&sock->tcb, sock->clientaddr.ip, sock->clientaddr.port, buf, len);
    return len;
}

struct socket *socket_get_listener(struct socket_addr *sockaddr, enum SOCKET_TYPE socktype) {
    if (socktype == SOCKTYPE_UDP)
        return socket_get_udp_listener(sockaddr);
    else if (socktype == SOCKTYPE_TCP)
        return socket_get_tcp_listener(sockaddr);

    return NULL;
}

static struct socket *socket_get_udp_listener(struct socket_addr *sockaddr) {
    for (uint8_t i = 0; i < AVAILABLE_SOCKETS_NUM; i++) {
        if (sockets[i].open && sockets[i].srcport == sockaddr->port)
            return &sockets[i];
    }
    return NULL;
}

static struct socket *socket_get_tcp_listener(struct socket_addr *sockaddr) {
     for (uint8_t i = 0; i < AVAILABLE_SOCKETS_NUM; i++) {
        if (sockets[i].srcport == sockaddr->port && (sockets[i].tcb.state < ESTABLISHED ||
            (sockets[i].tcb.state != CLOSED && sockets[i].clientaddr.ip == sockaddr->ip)))
            return &sockets[i];
    }
    return NULL;
}
