#include <stdint.h>
#include "arp.h"
#include "netcommon.h"

static void arp_memcpy(uint8_t *dest, uint8_t *src, uint32_t bytes) {
    for (uint32_t i = 0; i < bytes; i++)
        *dest++ = *src++;
}

static int32_t arp_memcmp(uint8_t *arr1, uint8_t *arr2, uint32_t bytes) {
    for (uint32_t i = 0; i < bytes; i++)
        if (*arr1++ != *arr2++)
            return 1;

    return 0;
}

void arp_request(uint8_t *buf, uint8_t *arppkt) {
    struct arphdr *hdr = (struct arphdr *) arppkt;
    struct arphdr *request = (struct arphdr *) buf;

    request->hwtype = hdr->hwtype;
    request->ptype = hdr->ptype;
    request->hwlen = hdr->hwlen;
    request->plen = hdr->plen;
    request->opcode = hton16(1);
    request->psender = hdr->psender;
    request->ptarget = hdr->ptarget;
    arp_memcpy(request->hwsender, hdr->hwsender, 6);
    arp_memcpy(request->hwtarget, hdr->hwtarget, 6);
}

void arp_reply(uint8_t *buf, uint8_t *arppkt, uint8_t *sender_mac) {
    struct arphdr *hdr = (struct arphdr *) arppkt;
    struct arphdr *reply = (struct arphdr *) buf;

    reply->hwtype = hdr->hwtype;
    reply->ptype = hdr->ptype;
    reply->hwlen = hdr->hwlen;
    reply->plen = hdr->plen;
    reply->opcode = hton16(2);
    reply->psender = hdr->ptarget;
    reply->ptarget = hdr->psender;
    arp_memcpy(reply->hwsender, sender_mac, 6);
    arp_memcpy(reply->hwtarget, hdr->hwsender, 6);
}