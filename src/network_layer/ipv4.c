#include <stdint.h>
#include <string.h>
#include "ipv4.h"
#include "arp.h"
#include "netcommon.h"

uint8_t itos(uint32_t val, char *buf);
uint32_t stoi(char *s);

static uint32_t ipv4_my_address;
static uint32_t ipv4_default_gateway;
static uint32_t ipv4_subnet_mask;

void ipv4_set_address(uint32_t ip) {
    ipv4_my_address = ip;
}

uint32_t ipv4_get_address(void) {
    return ipv4_my_address;
}

void ipv4_set_default_gateway(uint32_t gateway) {
    ipv4_default_gateway = gateway;
    arp_send_request(ipv4_get_address(), gateway);
}

uint32_t ipv4_get_default_gateway(void) {
    return ipv4_default_gateway;
}

void ipv4_set_subnet_mask(uint32_t mask) {
    ipv4_subnet_mask = mask;
}

uint32_t ipv4_get_subnet_mask(void) {
    return ipv4_subnet_mask;
}

uint16_t ipv4_options_len(struct ipv4hdr *hdr) {
    uint16_t len = hdr->ihl - (IPV4_MIN_HEADER_LEN / 4);
    len *= 4;  // convert from 32-bit fields to bytes
    return len;
}

uint16_t ipv4_data_len(struct ipv4hdr *hdr) {
    uint16_t header_len = hdr->ihl * 4;  // convert from 32-bit fields to bytes
    uint16_t total_len = hton16(hdr->len);
    return total_len - header_len;
}

void int_to_ipv4(uint32_t ip, char *ipv4) {
    uint8_t octet = (ip & 0xFF000000) >> 24;
    ipv4 += itos(octet, ipv4);
    *ipv4++ = '.';
    octet = (ip & 0xFF0000) >> 16;
    ipv4 += itos(octet, ipv4);
    *ipv4++ = '.';
    octet = (ip & 0xFF00) >> 8;
    ipv4 += itos(octet, ipv4);
    *ipv4++ = '.';
    octet = ip & 0xFF;
    ipv4 += itos(octet, ipv4);
    *ipv4++ = '\0';
}

uint32_t ipv4_to_int(char *ipv4) {
    uint32_t ip = 0;
    char copy[16];
    strcpy(copy, ipv4);
    char *start = copy;
    char *end = start;

    for (uint8_t i = 0; i < 4; i++) {
        while (*end != '.' && *end != '\0')
            end++;

        *end = '\0';
        ip = ip*256 + stoi(start);
        end++;
        start = end;
    }
    return ip;
}

uint8_t itos(uint32_t val, char *buf) {
    uint32_t i = 0;
    char copy[20];
    if (val == 0) {
        copy[i++] = '0';
    } else {
        while (val > 0) {
            copy[i++] = (val % 10) + '0';  // convert to ASCII number
            val /= 10;
        }
    }
    uint32_t j = 0;
    while (i > 0) {
        buf[j++] = copy[--i];
    }
    buf[j] = '\0';
    return j;
}

uint32_t stoi(char *s) {
    uint32_t i = 0;
    while (*s != '\0')
        i = 10*i + (*s++ - '0');
    return i;
}
