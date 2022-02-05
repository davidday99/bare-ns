#include <stdint.h>
#include "tcp.h"
#include "socket.h"
#include "netcommon.h"
#include "checksum.h"
#include "ipv4.h"
#include "string.h"

void tcp_deliver(uint8_t *data, uint8_t *pseudo) {
    // use srcip and destip to verify checksum
    struct tcphdr *hdr = (struct tcphdr *) data;
    struct pseudohdr *phdr = (struct pseudohdr *) pseudo;
    struct socket_addr sockaddr;
    sockaddr.ip = hton32(phdr->srcip);
    sockaddr.port = hton16(hdr->destport);
    struct socket *s = socket_get_listener(&sockaddr, SOCKTYPE_TCP);

    if (s == NULL) {
        return;
    }

    // if (calculate_tcp_checksum(data, pseudo) != 0) || !tcp_valid_segment(&s->tcb, hton32(hdr->seqnum)))
    //     return;
        
    struct tcphdr *txhdr = (struct tcphdr *) s->tcb.txbuf.ringbuf;
    memset((uint8_t *) txhdr, 0, sizeof(struct tcphdr));

    if (!tcp_valid_control_bits(&s->tcb, hdr)) {
        s->tcb.prevstate = s->tcb.state;
        s->tcb.state = CLOSED;
        txhdr->ctl |= RST;
    } else {
        // tcp_parse_options(&s->tcb, data);
        tcp_handle_control_bits(&s->tcb, data, pseudo);
    }

    if (txhdr->ctl != 0) {
        tcp_transmit_message(&s->tcb, hton32(phdr->srcip), hton16(hdr->srcport), 0, 0);
    }
}
