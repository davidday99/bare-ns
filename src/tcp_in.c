#include <stdint.h>
#include "tcp.h"
#include "socket.h"
#include "netcommon.h"
#include "checksum.h"
#include "ipv4.h"

uint16_t socket_write_buffer(struct socket *sock, uint8_t *buf, uint16_t len);

void tcp_deliver(uint8_t *data, uint8_t *pseudo) {
    // use srcip and destip to verify checksum
    struct tcphdr *hdr = (struct tcphdr *) data;
    struct pseudohdr *phdr = (struct pseudohdr *) pseudo;
    struct socket_addr sockaddr;
    sockaddr.ip = hton32(phdr->srcip);
    sockaddr.port = hton16(hdr->destport);
    struct socket *s = socket_get_listener(&sockaddr, SOCKTYPE_TCP);

    if (s == NULL)
        return;
        // send RST

    if (calculate_tcp_checksum(data, pseudo) != 0 || 
        !tcp_valid_segment(&s->tcb, hton32(hdr->seqnum)))
        return;
    
    switch (s->tcb.state) {
        case CLOSED:
            break;
        case LISTENING:
            tcp_handle_listening_state(&s->tcb, hdr);
            s->clientaddr.ip = sockaddr.ip;
            s->clientaddr.port = hton16(hdr->srcport);
            break;
        case SYN_RECEIVED:
            tcp_handle_syn_received_state(&s->tcb, hdr);
            break;
        case ESTABLISHED:
            tcp_handle_established_state(&s->tcb, hdr, &data[hdr->offset * 4], phdr);
        default:
            break;
    }

    if (s->tcb.transmit) {
        tcp_send(sockaddr.ip, s->tcb.txbuf.ringbuf, s->tcb.txbuf.wrptr);
        s->tcb.transmit = 0;
    }
}