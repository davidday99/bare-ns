#include <stdint.h>
#include "ipv4.h"
#include "netcommon.h"

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

uint8_t ipv4_options_len(struct ipv4hdr *hdr) {
    uint8_t len = hdr->ihl - (IPV4_MIN_HEADER_LEN / 4);
    len *= 4;  // convert from 32-bit fields to bytes
    return len;
}

uint8_t ipv4_data_len(struct ipv4hdr *hdr) {
    uint8_t header_len = hdr->ihl * 4;  // convert from 32-bit fields to bytes
    uint16_t total_len = hton16(hdr->len);
    return total_len - header_len;
}