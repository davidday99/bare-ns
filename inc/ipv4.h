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

uint8_t ipv4_options_len(struct ipv4hdr *hdr);
uint8_t ipv4_data_len(struct ipv4hdr *hdr);

#endif /* _IPV4_H_ */