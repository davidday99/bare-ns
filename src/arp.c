#include <stdint.h>
#include "arp.h"
#include "netcommon.h"
#include "stdlib.h"
#include "string.h"

#define ARP_HW_TYPE_ENET 0x0001
#define ARP_PTYPE_IPV4 0x0800

#define ARP_HW_ENET_LEN 6
#define ARP_P_IPV4_LEN 4

static int32_t arp_memcmp(uint8_t *arr1, uint8_t *arr2, uint32_t bytes) {
    for (uint32_t i = 0; i < bytes; i++)
        if (*arr1++ != *arr2++)
            return 1;

    return 0;
}

void arp_request(struct arphdr *request, uint8_t *hwsender, uint32_t psender,
                uint8_t *hwtarget, uint32_t ptarget) {
    request->hwtype = hton16(ARP_HW_TYPE_ENET);
    request->ptype = hton16(ARP_PTYPE_IPV4);
    request->hwlen = ARP_HW_ENET_LEN;
    request->plen = ARP_P_IPV4_LEN;
    request->opcode = hton16(ARP_OP_REQUEST);
    request->psender = hton32(psender);
    request->ptarget = hton32(ptarget);
    memcpy(request->hwsender, hwsender, 6);
    memcpy(request->hwtarget, hwtarget, 6);
}

void arp_reply(struct arphdr *reply, uint8_t *hwsender, uint32_t psender,
                uint8_t *hwtarget, uint32_t ptarget) {
    reply->hwtype = hton16(ARP_HW_TYPE_ENET);
    reply->ptype = hton16(ARP_PTYPE_IPV4);
    reply->hwlen = ARP_HW_ENET_LEN;
    reply->plen = ARP_P_IPV4_LEN;
    reply->opcode = hton16(ARP_OP_REPLY);
    reply->psender = hton32(psender);
    reply->ptarget = hton32(ptarget);
    memcpy(reply->hwsender, hwsender, 6);
    memcpy(reply->hwtarget, hwtarget, 6);
}
