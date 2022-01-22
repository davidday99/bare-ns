#include "tcp.h"
#include "netcommon.h"

void tcp_handle_listening_state(struct TCB *tcb, struct tcphdr *hdr) {
    struct tcphdr *tx_hdr = tcb->txbuf.ringbuf;
    if (hton16(hdr->ctl) & SYN) {
        tcb->state = SYN_RECEIVED;
        tx_hdr->ctl |= SYN;
        tx_hdr->ctl |= ACK;
        tcb->seqnum = 0;
        tcb->acknum = hdr->seqnum;  // add length of any data sent, although it usually won't include data
        tx_hdr->seqnum = tcb->seqnum;
        tx_hdr->acknum = tcb->acknum;
        tx_hdr->srcport = hdr->destport;
        tx_hdr->destport = hdr->srcport;
        tx_hdr->cksm = 0;
    }
}

void tcp_handle_syn_receive_state(struct TCB *tcb, struct tcphdr *hdr) {
    struct tcphdr *tx_hdr = tcb->txbuf.ringbuf;
    if (hton16(hdr->ctl) & ACK) {
        tcb->state = ESTABLISHED;
    }
}