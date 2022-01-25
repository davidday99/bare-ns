#include "tcp.h"
#include "netcommon.h"
#include "ipv4.h"
#include "checksum.h"
#include "socket.h"

#define TCP_WINDOW_SIZE 1024

uint16_t socket_write_buffer(struct socket *sock, uint8_t *buf, uint16_t len);

static void tcp_add_option(struct TCB *tcb, enum TCP_OPTIONS option, uint16_t value);

uint32_t tcp_get_isn(void);

void tcp_handle_listening_state(struct TCB *tcb, struct tcphdr *hdr) {
    struct tcphdr *tx_hdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    tx_hdr->ctl &= ~0x3F;  // clear all previously set bits
    tcb->seqnum = tcp_get_isn();
    tcb->acknum = hton32(hdr->seqnum) + 1;  // add length of any data sent, although it usually won't include data
    tx_hdr->srcport = hdr->destport;
    tx_hdr->destport = hdr->srcport;
    tx_hdr->seqnum = hton32(tcb->seqnum);
    tx_hdr->acknum = hton32((hton32(hdr->seqnum) + 1));
    tx_hdr->window = hton16(TCP_WINDOW_SIZE);  // TODO: determine whether this starting value will work
    tx_hdr->cksm = 0;
    tx_hdr->urgent = 0;
    tcb->txbuf.wrptr = sizeof(struct tcphdr);
    tcb->transmit = 1;

    if (hdr->ctl == SYN) {
        tcb->state = SYN_RECEIVED;
        tx_hdr->ctl |= SYN;
        tx_hdr->ctl |= ACK;
        tx_hdr->offset = 6;
        tcp_add_option(tcb, MSS, 1024);
    } else {
        tx_hdr->ctl |= RST;
        tx_hdr->offset = 5;
    }
}

void tcp_handle_syn_received_state(struct TCB *tcb, struct tcphdr *hdr) {
    struct tcphdr *tx_hdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    tx_hdr->ctl &= ~0x3F;  // clear all previously set bits
    if (hdr->ctl & ACK) {
        tcb->state = ESTABLISHED;
    } else {
        tcb->state = LISTENING;
        // send RST message
    }
}

void tcp_handle_established_state(struct TCB *tcb, struct tcphdr *hdr, uint8_t *data, struct pseudohdr *phdr) {
    struct tcphdr *tx_hdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    tx_hdr->ctl &= ~0x3F;  // clear all previously set bits
    if (hdr->ctl & PSH) {
        struct socket_addr sockaddr = {hton32(phdr->srcip), hton16(hdr->destport)};
        struct socket *s = socket_get_listener(&sockaddr, SOCKTYPE_TCP);
        uint16_t len = hton16(phdr->len) - hdr->offset*4;
        socket_write_buffer(s, data, len);
        tcb->acknum += len;
        tx_hdr->srcport = hdr->destport;
        tx_hdr->destport = hdr->srcport;
        tx_hdr->seqnum = hton32(tcb->seqnum);
        tx_hdr->acknum = hton32(tcb->acknum);
        tx_hdr->offset = 5;
        tx_hdr->ctl |= ACK;
        tx_hdr->window = hton16(TCP_WINDOW_SIZE);  // TODO: determine whether this starting value will work
        tx_hdr->cksm = 0;
        tx_hdr->urgent = 0;
        tcb->txbuf.wrptr = sizeof(struct tcphdr);
        tcb->transmit = 1;
    }
}

uint16_t calculate_tcp_checksum(uint8_t *tcpdata, uint8_t *pseudo) {
    struct pseudohdr *phdr = (struct pseudohdr *) pseudo;
    uint16_t sum1 = ones_complement_sum_buffer(tcpdata, hton16(phdr->len));
    uint16_t sum2 = ones_complement_sum_buffer(pseudo, sizeof(struct pseudohdr));

    uint16_t sum = ones_complement_sum(sum1, sum2);
    return ~sum;
}


static void tcp_add_option(struct TCB *tcb, enum TCP_OPTIONS option, uint16_t value) {
    uint8_t *options = (uint8_t *) &tcb->txbuf.ringbuf[tcb->txbuf.wrptr];
    switch (option) {
        case MSS:
            *options++ = 2;
            *options++ = 4;
            *options++ = (value & 0xFF00) >> 8;
            *options++ = value & 0xFF;
            tcb->txbuf.wrptr += 4;
            break;
    }
}

uint32_t tcp_get_isn(void) {
    return 0;
}

uint8_t tcp_valid_segment(struct TCB *tcb, uint32_t seqnum) {
    if (seqnum == tcb->acknum || tcb->state == LISTENING)
        return 1;
    else 
        return 0;
}