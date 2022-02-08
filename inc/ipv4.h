#ifndef _IPV4_H_
#define _IPV4_H_

#include <stdint.h>

#define IP_VERSION_4 0x04

#define IPV4_PRECEDENCE_ROUTINE 0
#define IPV4_PRECEDENCE_PRIORITY 0x20
#define IPV4_PRECEDENCE_IMMEDIATE 0x40
#define IPV4_PRECEDENCE_FLASH 0x60
#define IPV4_PRECEDENCE_FLASH_OVERRIDE 0x80
#define IPV4_PRECEDENCE_CRITICAL 0xA0
#define IPV4_PRECEDENCE_INTERNETWORK_CTRL 0xC0
#define IPV4_PRECEDENCE_NETWORK_CTRL 0xE0
#define IPV4_PRECEDENC_M 0xE0

#define IPV4_TOS_DELAY_NORMAL 0
#define IPV4_TOS_THROUGHPUT_NORMAL 0
#define IPV4_TOS_RELIABILITY_NORMAL 0
#define IPV4_TOS_DELAY_LOW 0x80
#define IPV4_TOS_THROUGHPUT_HIGH 0x10
#define IPV4_TOS_RELIABILITY_HIGH 0x20
#define IPV4_TOS_M 0x1F

#define IPV4_OPTIONS_OFFSET 20

#define IPV4_MIN_HEADER_LEN 20

#define IPV4_PROTOCOL_ICMP 0x01
#define IPV4_PROTOCOL_TCP 0x06
#define IPV4_PROTOCOL_UDP 0x11

struct ipv4hdr {
    uint8_t ihl : 4;
    uint8_t version : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t frag_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t cksm;
    uint32_t src;
    uint32_t dest;
};

struct pseudohdr {
    uint32_t srcip;
    uint32_t destip;
    uint8_t zero;
    uint8_t pctl;
    uint16_t len;
};

void ipv4_set_address(uint32_t ip);
uint32_t ipv4_get_address(void);
void ipv4_set_default_gateway(uint32_t gateway);
uint32_t ipv4_get_default_gateway(void);
void ipv4_set_subnet_mask(uint32_t mask);
uint32_t ipv4_get_subnet_mask(void);
uint16_t ipv4_options_len(struct ipv4hdr *hdr);
uint16_t ipv4_data_len(struct ipv4hdr *hdr);
void int_to_ipv4(uint32_t ip, char *ipv4);
uint32_t ipv4_to_int(char *ipv4);

void ipv4_deliver(uint8_t *buf);
void ipv4_send(uint32_t destip, uint8_t *data, uint16_t len, uint8_t protcol);

#endif /* _IPV4_H_ */