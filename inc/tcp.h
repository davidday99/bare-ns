#ifndef _TCP_H_
#define _TCP_H_

#include <stdint.h>

#define TCP_BUF_LEN 1024

struct tcphdr {
    uint16_t srcport;
    uint16_t destport;
    uint32_t seqnum;
    uint32_t acknum;
    uint8_t reserved : 4;
    uint8_t offset : 4;
    uint8_t ctl;
    uint16_t window;
    uint16_t cksm;
    uint16_t urgent;
};

struct tcppseudohdr {
    uint32_t srcip;
    uint32_t destip;
    uint8_t zero;
    uint8_t pctl;
    uint16_t len;
};

enum TCP_CONTROL_BITS {
    FIN = 1,
    SYN = 2,
    RST = 4,
    PSH = 8,
    ACK = 16, 
    URG = 32
};

enum TCP_OPTIONS {
    END,
    NOP,
    MSS
};

enum TCP_STATES {
    LISTENING,
    SYN_SENT,
    SYN_RECEIVED,
    ESTABLISHED,
    FIN_WAIT_1,
    FIN_WAIT_2,
    CLOSE_WAIT,
    CLOSING,
    LAST_ACK,
    TIME_WAIT,
    CLOSED
};

struct tcp_buffer {
    uint8_t ringbuf[TCP_BUF_LEN];
    uint16_t rdptr;
    uint16_t wrptr;
};

struct TCB {
    enum TCP_STATES state;
    uint32_t snd_una;
    uint32_t seqnum;
    uint32_t acknum;
    uint16_t window;
    struct tcp_buffer txbuf;
    struct tcp_buffer rxbuf;
};

void tcp_deliver(uint8_t *payload, uint32_t srcip, uint32_t destip, uint16_t len);

void tcp_send(uint8_t *data, uint16_t len);

void tcp_handle_listening_state(struct TCB *tcb, struct tcphdr *hdr);
void tcp_handle_syn_receive_state(struct TCB *tcb, struct tcphdr *hdr);

#define TCP_DATA_OFFSET 176

#endif /* _TCP_H_ */