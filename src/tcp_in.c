#include <stdint.h>
#include "tcp.h"
#include "socket.h"
#include "netcommon.h"

uint16_t socket_write_buffer(struct socket *sock, uint8_t *buf, uint16_t len);

void tcp_deliver(uint8_t *payload, uint32_t srcip, uint32_t destip, uint16_t len) {
    // use srcip and destip to verify checksum
    struct tcphdr *hdr = (struct tcphdr *) payload;
    struct socket_addr sockaddr;
    sockaddr.ip = srcip;
    sockaddr.port = hton16(hdr->destport);
    struct socket *s = socket_get_listener(&sockaddr, SOCKTYPE_TCP);

    if (s == NULL)
        return;
        // send RST

    switch (s->tcb.state) {
        case CLOSED:
            break;
        case LISTENING:
            tcp_handle_listening_state(&s->tcb, hdr);
            break;
        case SYN_RECEIVED:
            tcp_handle_syn_received_state(&s->tcb, hdr);
    }
}