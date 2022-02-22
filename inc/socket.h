#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <stdint.h>
#include "tcp.h"

#define SOCKBUF_LEN 1000
#define AVAILABLE_SOCKETS_NUM 4
#define NULL 0
#define SOCKET_OPEN 1
#define SOCKET_NOT_OPEN 0

#define SOCKADDR_IP_ANY 0
#define IP_METADATA_SIZE 8

enum SOCKET_TYPE {
    SOCKTYPE_TCP,
    SOCKTYPE_UDP
};

struct socket_buffer {
    uint8_t ringbuf[SOCKBUF_LEN];
    uint16_t rdptr;
    uint16_t wrptr;
};

struct socket_addr {
    uint32_t ip;
    uint16_t port;
};

struct socket {
    struct socket_buffer sockbuf;
    struct TCB tcb;
    uint16_t srcport;
    enum SOCKET_TYPE socktype;
    struct socket_addr clientaddr;
    uint8_t open;
};

struct socket *socket_init(enum SOCKET_TYPE socktype);
void socket_bind(struct socket *sock, uint16_t port);
uint8_t socket_accept(struct socket *sock);
void socket_close(struct socket *sock);
uint16_t socket_recv(struct socket *sock, struct socket_addr *sockaddr, uint8_t *buf, uint16_t len);
uint16_t socket_read(struct socket *sock, uint8_t *buf, uint16_t len);
void socket_sendto(struct socket *sock, struct socket_addr *sockaddr, uint8_t *buf, uint16_t len);
uint16_t socket_send(struct socket *sock, uint8_t *buf, uint16_t len);
struct socket *socket_get_listener(struct socket_addr *sockaddr, enum SOCKET_TYPE socktype);

#endif /* _SOCKET_H_ */