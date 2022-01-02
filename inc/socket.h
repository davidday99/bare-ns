#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <stdint.h>

#define SOCKBUF_LEN 500
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
    uint8_t rdptr;
    uint8_t wrptr;
};

struct socket {
    struct socket_buffer sockbuf;
    uint32_t dest;
    uint16_t srcport;
    enum SOCKET_TYPE socktype;
    uint8_t open;
};

struct socket_addr {
    uint32_t ip;
    uint16_t port;
};

struct socket *socket_init(enum SOCKET_TYPE socktype);
void socket_bind(struct socket *sock, struct socket_addr *sockaddr);
void socket_close(struct socket *sock);
uint16_t socket_read(struct socket *sock, struct socket_addr *sockaddr, uint8_t *buf, uint16_t len);
void socket_send(struct socket *sock, uint8_t *buf, uint16_t len);
struct socket *socket_get_listener(struct socket_addr *sockaddr, enum SOCKET_TYPE socktype);

#endif /* _SOCKET_H_ */