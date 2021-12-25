#include <stdint.h>
#include <string.h>
#include "arp.h"
#include "netcommon.h"

uint8_t arp_header[] = {
    0x00,
    0x01,  // HW type, 1 (ethernet)
    0x08,
    0x00,  // Protocol type, 0x0800 (ipv4)
    0x06,  // HW len, 6 byte MAC
    0x04,  // Protocol len, 4 byte ip
    0x00,
    0x01,  // arp operation, request
    0xDE,
    0xAD,
    0xBE,
    0xEF,
    0xCC,
    0xCC,  // Sender HW addr, DE-AD-BE-EF-CC-CC
    0xC0,
    0xA8,
    0x00,
    0x6F,  // Sender protocol addr, 192.168.0.111
    0xAB,
    0xCD,
    0xEF,
    0x01,
    0x23,
    0x45,  // Target HW addr, AB-CD-EF-01-23-45
    0xC0,
    0xA8,
    0x00,
    0x01  // Target protocol addr, 192.168.0.1
};

int test_read_arp_hdr() {
    int success = 1;
    struct arphdr *hdr = (struct arphdr *) arp_header;

    uint8_t hwsender[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xCC, 0xCC};
    uint8_t hwtarget[] = {0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45};
    
    success &= hdr->hwtype == hton16(ARP_HW_TYPE_ENET);
    success &= hdr->ptype == hton16(ARP_PTYPE_IPV4);
    success &= hdr->hwlen == ARP_HW_ENET_LEN;
    success &= hdr->plen == ARP_P_IPV4_LEN;
    success &= hdr->opcode == hton16(ARP_OP_REQUEST);
    success &= hdr->psender == hton32(0xC0A8006F);
    success &= memcmp(hdr->hwsender, hwsender, 6) == 0;
    success &= hdr->ptarget == hton32(0xC0A80001);
    success &= memcmp(hdr->hwtarget, hwtarget, 6) == 0;
    return success;
}

int test_arp_request() {
    int success = 1;
    uint8_t output[sizeof(struct arphdr)];
    struct arphdr *hdr = (struct arphdr *) output;
    
    uint8_t hwsender[] = {0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45};
    uint8_t hwtarget[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    arp_request(hdr, hwsender, 0x012345CC, hwtarget, 0xABCDEF12);

    success &= hdr->hwtype == hton16(ARP_HW_TYPE_ENET);
    success &= hdr->ptype == hton16(ARP_PTYPE_IPV4);
    success &= hdr->hwlen == 6;
    success &= hdr->plen == 4;
    success &= hdr->opcode == hton16(ARP_OP_REQUEST);
    success &= hdr->psender == hton32(0x012345CC);
    success &= memcmp(hdr->hwsender, hwsender, 6) == 0;
    success &= hdr->ptarget == hton32(0xABCDEF12);
    success &= memcmp(hdr->hwtarget, hwtarget, 6) == 0;
    return success;
}

int test_arp_reply() {
    int success = 1;
    uint8_t output[sizeof(struct arphdr)];
    struct arphdr *hdr = (struct arphdr *) output;
    
    uint8_t hwsender[] = {0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45};
    uint8_t hwtarget[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    arp_reply(hdr, hwsender, 0x012345CC, hwtarget, 0xABCDEF12);

    success &= hdr->hwtype == hton16(ARP_HW_TYPE_ENET);
    success &= hdr->ptype == hton16(ARP_PTYPE_IPV4);
    success &= hdr->hwlen == 6;
    success &= hdr->plen == 4;
    success &= hdr->opcode == hton16(ARP_OP_REPLY);
    success &= hdr->psender == hton32(0x012345CC);
    success &= memcmp(hdr->hwsender, hwsender, 6) == 0;
    success &= hdr->ptarget == hton32(0xABCDEF12);
    success &= memcmp(hdr->hwtarget, hwtarget, 6) == 0;
    return success;
}

int test_arp() {
    int success = 1;
    success &= test_read_arp_hdr();
    success &= test_arp_request();
    success &= test_arp_reply();
    return success;
}