#include "tcp.h"
#include "netcommon.h"
#include "ipv4.h"
#include "checksum.h"
#include "socket.h"
#include "string.h"

#define TCP_WINDOW_SIZE 1024

uint16_t socket_write_buffer(struct socket *sock, uint8_t *buf, uint16_t len);

static void tcp_add_option(struct TCB *tcb, enum TCP_OPTION option, uint16_t value, uint8_t opt_count);

static uint32_t tcp_get_isn(void);

static void tcp_handle_rst(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr);
static void tcp_handle_urg(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr);
static void tcp_handle_ack(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr);
static void tcp_handle_psh(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr);
static void tcp_handle_syn(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr);
static void tcp_handle_fin(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr);

static enum TCP_OPTION tcp_get_option(uint8_t *data, uint8_t **ptr);


uint16_t calculate_tcp_checksum(uint8_t *tcpdata, uint8_t *pseudo) {
    struct pseudohdr *phdr = (struct pseudohdr *) pseudo;
    uint16_t sum1 = ones_complement_sum_buffer(tcpdata, hton16(phdr->len));
    uint16_t sum2 = ones_complement_sum_buffer(pseudo, sizeof(struct pseudohdr));

    uint16_t sum = ones_complement_sum(sum1, sum2);
    static int x;
    if (~sum == 0x0c44) {
        x++;
    }
    return ~sum;
}


static void tcp_add_option(struct TCB *tcb, enum TCP_OPTION option, uint16_t value, uint8_t opt_count) {
    uint8_t *options = (uint8_t *) &tcb->txbuf.ringbuf[TCP_HEADER_LEN + opt_count];
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

void *tcp_parse_options(struct TCB *tcb, uint8_t *data) {
    uint8_t *ptr = 0;
    enum TCP_OPTION opt;
    while ((opt = tcp_get_option(data, &ptr)) != END) {
        // set variables inside TCB as needed
        if (opt == NOP) {
            continue;
        } else if (opt = MSS) {
            uint16_t mss = (*(ptr + 2) << 8) | *(ptr + 3);
        }
    }
}

static enum TCP_OPTION tcp_get_option(uint8_t *data, uint8_t **ptr) {
    if (*ptr == 0) {
        *ptr = &data[TCP_HEADER_LEN];
    } else {
        switch (**ptr) {
            case END:
                break;
            case NOP:
                *ptr += 1;
                break;
            case MSS:
                *ptr += 4;
                break;
        }
    }
    return **ptr;
}

static uint32_t tcp_get_isn(void) {
    return 0;
}

uint8_t tcp_valid_segment(struct TCB *tcb, uint32_t seqnum) {
    if (seqnum == tcb->acknum || tcb->state == LISTENING)
        return 1;
    else 
        return 0;
}

void tcp_handle_control_bits(struct TCB *tcb, uint8_t *data, uint8_t *pseudo) {
    struct tcphdr *hdr = (struct tcphdr *) data;
    struct pseudohdr *phdr = (struct pseudohdr *) pseudo;

    if (hdr->ctl & RST) {
        tcp_handle_rst(tcb, data, phdr);
    }
    if (hdr->ctl & URG) {
        tcp_handle_urg(tcb, data, phdr);
    }
    if (hdr->ctl & ACK) {
        tcp_handle_ack(tcb, data, phdr);
    }
    if (hdr->ctl & PSH) {
        tcp_handle_psh(tcb, data, phdr);
    }
    if (hdr->ctl & SYN) {
        tcp_handle_syn(tcb, data, phdr);
    }
    if (hdr->ctl & FIN) {
        tcp_handle_fin(tcb, data, phdr);
    }
}

static void tcp_handle_rst(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr) {
    tcb->prevstate = tcb->state;
    tcb->state = CLOSED;
    return;
}

static void tcp_handle_urg(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr) {
    // TODO: implement
    return;
}

static void tcp_handle_ack(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr) {
    struct tcphdr *hdr = (struct tcphdr *) data;
    if (tcb->state == SYN_RECEIVED) {
        tcb->prevstate = tcb->state;
        struct socket_addr sockaddr = {hton32(phdr->srcip), hton16(hdr->destport)};
        struct socket *s = socket_get_listener(&sockaddr, SOCKTYPE_TCP);
        s->clientaddr.ip = hton32(phdr->srcip);
        s->clientaddr.port = hton16(hdr->srcport);
        tcb->state = ESTABLISHED;
    }
    /* clear retransmission buffer of any newly ACK'd data */
    return;
}

static void tcp_handle_psh(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr) {
    struct tcphdr *txhdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    struct tcphdr *hdr = (struct tcphdr *) data;
    struct socket_addr sockaddr = {hton32(phdr->srcip), hton16(hdr->destport)};
    struct socket *s = socket_get_listener(&sockaddr, SOCKTYPE_TCP);
    uint16_t len = hton16(phdr->len) - hdr->offset*4;
    socket_write_buffer(s, &data[hdr->offset*4], len);
    tcb->acknum += len;
    txhdr->ctl |= ACK;
    txhdr->acknum = hton32(tcb->acknum);
}

static void tcp_handle_syn(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr) {
    struct tcphdr *txhdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    struct tcphdr *hdr = (struct tcphdr *) data;
    tcb->prevstate = tcb->state;
    tcb->state = SYN_RECEIVED;
    tcb->seqnum = tcp_get_isn();
    tcb->next = tcb->seqnum + 1;
    tcb->acknum = hton32(hdr->seqnum) + 1;
    txhdr->ctl |= SYN;
    txhdr->ctl |= ACK;
    tcp_add_option(tcb, MSS, 1024, 0);
    txhdr->offset = 6;
}

static void tcp_handle_fin(struct TCB *tcb, uint8_t *data, struct pseudohdr *phdr) {
    struct tcphdr *txhdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    struct tcphdr *hdr = (struct tcphdr *) data;
    tcb->prevstate = tcb->state;
    tcb->state = CLOSE_WAIT;
    txhdr->ctl |= ACK;
}

uint8_t tcp_valid_control_bits(struct TCB *tcb, struct tcphdr *hdr) {
    uint8_t valid = 1;
    switch (tcb->state) {
        case CLOSED:
            valid = 0;
            break;
        case LISTENING:
            if (hdr->ctl != SYN)
                valid = 0;
            break;
        case SYN_SENT:
            if (hdr->ctl != (SYN | ACK)) 
                valid = 0;
            break;
        case SYN_RECEIVED:
            if (!(hdr->ctl & ACK) || ((hdr->ctl == RST) && tcb->prevstate != LISTENING))
                valid = 0;
            break;
        case ESTABLISHED:
            break;
        case FIN_WAIT_1:
            if (hdr->ctl != FIN && hdr->ctl != ACK)
                valid = 0;
            break;
        case FIN_WAIT_2:
            if (hdr->ctl != FIN)
                valid = 0;
            break;
        case CLOSE_WAIT:
            if (hdr->ctl != ACK)
                valid = 0;
            break;
        case CLOSING:
        case LAST_ACK:
            if (hdr->ctl != ACK)
                valid = 0;
            break;
        case TIME_WAIT:
            valid = 0;
            break;
        default:
            valid = 0;
            break;
    }
    return valid;
}

void tcp_transmit_message(struct TCB *tcb, uint32_t destip, uint16_t destport, uint8_t *data, uint16_t len) {
    struct tcphdr *txhdr = (struct tcphdr *) tcb->txbuf.ringbuf;
    txhdr->seqnum = hton32(tcb->seqnum);
    txhdr->acknum = hton32(tcb->acknum);
    txhdr->cksm = 0;
    tcb->next += len;
    tcb->seqnum = tcb->next;
    
    if (txhdr->offset == 0) {
        txhdr->offset = 5;
    }
    if (txhdr->window == 0) {
        txhdr->window = hton16(TCP_WINDOW_SIZE);
    }

    if (len > 0 && len <= UINT16_MAX - tcb->txbuf.wrptr - TCP_HEADER_LEN)
        txhdr->ctl |= PSH;
        memcpy(&tcb->txbuf.ringbuf[tcb->txbuf.wrptr + TCP_HEADER_LEN], data, len);

    tcp_send(tcb->srcport, destip, destport, tcb->txbuf.ringbuf, TCP_HEADER_LEN + tcb->txbuf.wrptr + len);
    tcb->txbuf.wrptr = 0;
}