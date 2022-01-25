#include "tcp.h"
#include "netcommon.h"
#include "ipv4.h"
#include "checksum.h"

void tcp_add_option(struct TCB *tcb, enum TCP_OPTIONS option, uint16_t value);

void tcp_handle_listening_state(struct TCB *tcb, struct tcphdr *hdr) {
    struct tcphdr *tx_hdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    if (hdr->ctl & SYN) {
        tcb->state = SYN_RECEIVED;
        tcb->seqnum = 0;
        tcb->acknum = hton32(hdr->seqnum);  // add length of any data sent, although it usually won't include data
        tx_hdr->srcport = hdr->destport;
        tx_hdr->destport = hdr->srcport;
        tx_hdr->ctl &= ~0x3F;  // clear all previously set bits
        tx_hdr->ctl |= SYN;
        tx_hdr->ctl |= ACK;
        tx_hdr->seqnum = 0;
        tx_hdr->acknum = hton32((hton32(hdr->seqnum) + 1));
        tx_hdr->offset = 6;
        tx_hdr->window = 0xFFFF;  // TODO: determine whether this starting value will work
        tx_hdr->cksm = 0;
        tx_hdr->urgent = 0;
        tcb->txbuf.wrptr = sizeof(struct tcphdr);
        tcp_add_option(tcb, MSS, 1024);
        tcb->transmit = 1;
    }
}

void tcp_handle_syn_received_state(struct TCB *tcb, struct tcphdr *hdr) {
    struct tcphdr *tx_hdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    if (hdr->ctl == ACK) {
        tcb->state = ESTABLISHED;
    } else {
        tcb->state = LISTENING;
        // send RST message
    }
}

uint16_t calculate_tcp_checksum(uint8_t *tcpdata, uint8_t *pseudo) {
    struct pseudohdr *phdr = (struct pseudohdr *) pseudo;
    uint16_t sum1 = ones_complement_sum_buffer(tcpdata, hton16(phdr->len));
    uint16_t sum2 = ones_complement_sum_buffer(pseudo, sizeof(struct pseudohdr));

    uint16_t sum = ones_complement_sum(sum1, sum2);
    return ~sum;
}


void tcp_add_option(struct TCB *tcb, enum TCP_OPTIONS option, uint16_t value) {
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