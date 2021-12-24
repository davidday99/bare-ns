#include <stdint.h>
#include <string.h>
#include "arp.h"
#include "netcommon.h"

int test_arp_request() {
    int success = 1;
    uint8_t output[sizeof(struct arphdr)];
    struct arphdr *hdr = (struct arphdr *) output;
    
    uint8_t hwsender[] = {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56};
    uint8_t hwtarget[] = {0, 0, 0, 0, 0, 0};

    arp_request(hdr, hwsender, 0x012345CC, hwtarget, 0xABCDEF12);

    success &= hdr->hwtype == hton16(ARP_OP_REQUEST);
    success &= hdr->ptype == hton16(0x0800);
    success &= hdr->hwlen == 6;
    success &= hdr->plen == 4;
    success &= hdr->opcode == hton16(ARP_OP_REQUEST);
    success &= hdr->psender == hton32(0x012345CC);
    success &= memcmp(hdr->hwsender,(uint8_t []) {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56}, 6) == 0;
    success &= hdr->ptarget == hton32(0xABCDEF12);
    success &= memcmp(hdr->hwtarget, (uint8_t []) {0, 0, 0, 0, 0, 0}, 6) == 0;
    return success;
}

int test_arp_reply() {
    int success = 1;
    uint8_t output[sizeof(struct arphdr)];
    struct arphdr *hdr = (struct arphdr *) output;
    
    uint8_t hwsender[] = {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56};
    uint8_t hwtarget[] = {0, 0, 0, 0, 0, 0};

    arp_reply(hdr, hwsender, 0x012345CC, hwtarget, 0xABCDEF12);

    success &= hdr->hwtype == hton16(ARP_OP_REQUEST);
    success &= hdr->ptype == hton16(0x0800);
    success &= hdr->hwlen == 6;
    success &= hdr->plen == 4;
    success &= hdr->opcode == hton16(ARP_OP_REPLY);
    success &= hdr->psender == hton32(0x012345CC);
    success &= memcmp(hdr->hwsender,(uint8_t []) {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56}, 6) == 0;
    success &= hdr->ptarget == hton32(0xABCDEF12);
    success &= memcmp(hdr->hwtarget, (uint8_t []) {0, 0, 0, 0, 0, 0}, 6) == 0;
    return success;
}

int test_arp() {
    int success = 1;
    success &= test_arp_request();
    success &= test_arp_reply();
    return success;
}