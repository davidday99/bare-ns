#include <stdint.h>
#include <string.h>
#include "arp.h"
#include "netcommon.h"

int test_arp_request() {
    int success = 1;
    struct arphdr hdr;
    uint8_t output[sizeof(struct arphdr)];

    hdr.hwtype = 1;
    hdr.ptype = 0x0800;
    hdr.hwlen = 6;
    hdr.plen = 4;
    hdr.psender = 0xCCCCCCCC;
    memcpy(hdr.hwsender, (uint8_t []) {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56}, 6);
    hdr.ptarget = 0xAAAAAAAA;
    memcpy(hdr.hwtarget, (uint8_t []) {0, 0, 0, 0, 0, 0}, 6);

    arp_request(output, (uint8_t *) &hdr);

    struct arphdr *outhdr = (struct arphdr *) output;

    success &= outhdr->hwtype == 1;
    success &= outhdr->ptype == 0x0800;
    success &= outhdr->hwlen == 6;
    success &= outhdr->plen == 4;
    success &= outhdr->opcode == hton16(ARP_OP_REQUEST);
    success &= outhdr->psender == 0xCCCCCCCC;
    success &= memcmp(outhdr->hwsender,(uint8_t []) {0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56}, 6) == 0;
    success &= outhdr->ptarget == 0xAAAAAAAA;
    success &= memcmp(outhdr->hwtarget, (uint8_t []) {0, 0, 0, 0, 0, 0}, 6) == 0;
    return success;
}

int test_arp() {
    int success = 1;
    success &= test_arp_request();
    return success;
}