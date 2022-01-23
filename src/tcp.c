#include "tcp.h"
#include "netcommon.h"

void tcp_handle_listening_state(struct TCB *tcb, struct tcphdr *hdr) {
    struct tcphdr *tx_hdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    if (hdr->ctl & SYN) {
        tcb->state = SYN_RECEIVED;
        tcb->seqnum = 0;
        tcb->acknum = hdr->seqnum;  // add length of any data sent, although it usually won't include data
        tx_hdr->srcport = hdr->destport;
        tx_hdr->destport = hdr->srcport;
        tx_hdr->ctl &= ~0x3F;  // clear all previously set bits
        tx_hdr->ctl |= SYN;
        tx_hdr->ctl |= ACK;
        tx_hdr->seqnum = tcb->seqnum;
        tx_hdr->acknum = tcb->acknum;
        tx_hdr->offset = 0;
        tx_hdr->window = 0xFFFF;  // TODO: determine whether this starting value will work
        tx_hdr->cksm = 0;
        tx_hdr->urgent = 0;
        tcb->txbuf.wrptr = sizeof(struct tcphdr);
    }
}

void tcp_handle_syn_received_state(struct TCB *tcb, struct tcphdr *hdr) {
    struct tcphdr *tx_hdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    if (hton16(hdr->ctl) & ACK) {
        tcb->state = ESTABLISHED;
    }
}