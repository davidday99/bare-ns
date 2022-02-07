#include <stdint.h>
#include "tcp.h"
#include "socket.h"
#include <string.h>

int test_tcp_socket_send() {
    int success = 1;

    struct socket *s = socket_init(SOCKTYPE_TCP);
    socket_bind(s, 80);

    uint8_t data[2] = {0xBE, 0xEF};
    s->tcb.state = ESTABLISHED ;
    s->clientaddr.ip = 0xC0A80001;
    s->clientaddr.port = 0xC0C1;

    socket_send(s, data, 2);
    socket_close(s);

    success &= memcmp(&s->tcb.txbuf.ringbuf[TCP_HEADER_LEN], data, sizeof(data)) == 0;

    return success;
}