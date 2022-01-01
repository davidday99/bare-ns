#include <stdint.h>
#include <string.h>
#include "ipv4.h"
#include "netcommon.h"

uint8_t ip_packet[] = {
    0x46,  // version 4, IHL 6 (20 bytes header + 4 bytes options),
    0x01,  // TOS 1 (not sure what typical values are for this field)
    0x00,
    0x1A,  // Total len, 26 bytes (20 bytes header + 4 bytes options + 2 bytes data)
    0xAB,
    0XCD,  // ID, 0xABCD
    0x4F,
    0xAA,  // flags = 010, offset = 0b0111110101010
    0x40,  // TTL, 64
    0x06,  // Protocl, 0x06 (TCP)
    0xAB,
    0xCD,  // Header checksum
    0xC0,
    0xA8,
    0x00,
    0x6F,  // Src ip, 192.168.0.111
    0xC0,
    0xA8,
    0x00,
    0x01,  // Dest ip, 192168.0.1
    0xDE,
    0xAD,
    0xBE,
    0xEF,  // Options, 4 bytes 0xDEADBEEF
    0xA0,
    0xA0,  // data, 2 bytes 0xA0A0
};

int test_ipv4_to_int() {
    int success = 1;
    char *ipv4 = "192.168.0.1";
    success = ipv4_to_int(ipv4) == 0xC0A80001;
    return success;
}


int test_read_ip_header() {
    int success = 1;
    struct ipv4hdr *hdr = (struct ipv4hdr *) ip_packet;

    uint8_t options[] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t data[] = {0xA0, 0xA0};

    success &= hdr->ihl == 6;
    success &= hdr->version == IP_VERSION_4;
    success &= hdr->tos == 1;
    success &= hdr->len == hton16(0x1A);
    success &= hdr->id == hton16(0xABCD);
    success &= hdr->frag_offset == hton16(0x4FAA);
    success &= hdr->ttl == 0x40;
    success &= hdr->protocol == 0x06;
    success &= hdr->cksm == hton16(0xABCD);
    success &= hdr->src == hton32(0xC0A8006F);
    success &= hdr->dest == hton32(0xC0A80001);
    success &= memcmp(ip_packet + IPV4_OPTIONS_OFFSET, options, 4) == 0;
    success &= memcmp(ip_packet + IPV4_OPTIONS_OFFSET + 4, data, 2) == 0;


    return success;
}

int test_get_ip_options_len() {
    int success = 1;
    struct ipv4hdr *hdr = (struct ipv4hdr *) ip_packet;
    success &= ipv4_options_len(hdr) == 4;
    return success;
}

int test_get_ip_data_len() {
    int success = 1;
    struct ipv4hdr *hdr = (struct ipv4hdr *) ip_packet;
    success &= ipv4_data_len(hdr) == 2;
    return success;
}

int test_ip() {
    int success = 1;
    success &= test_ipv4_to_int();
    success &= test_read_ip_header();
    success &= test_get_ip_options_len();
    success &= test_get_ip_data_len();
    return success;
}