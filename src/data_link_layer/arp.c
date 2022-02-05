#include <stdint.h>
#include "arp.h"
#include "ethernet.h"
#include "ipv4.h"
#include "net.h"
#include "netcommon.h"
#include <string.h>

static uint8_t MAC[] = {0xA0, 0xCD, 0xEF, 0x01, 0x23, 0x45};

void arp_send_request(uint8_t *hwsender, uint32_t psender,
                uint8_t *hwtarget, uint32_t ptarget) {
    uint8_t arppkt[ARP_SIZE];
    struct arphdr *request = (struct arphdr *) arppkt;
    request->hwtype = hton16(ARP_HW_TYPE_ENET);
    request->ptype = hton16(ARP_PTYPE_IPV4);
    request->hwlen = ARP_HW_ENET_LEN;
    request->plen = ARP_P_IPV4_LEN;
    request->opcode = hton16(ARP_OP_REQUEST);
    request->psender = hton32(psender);
    request->ptarget = hton32(ptarget);
    memcpy(request->hwsender, hwsender, 6);
    memcpy(request->hwtarget, hwtarget, 6);
    net_tx(request->hwtarget, arppkt, sizeof(arppkt), ETHERTYPE_ARP);
}

void arp_send_reply(uint8_t *hwsender, uint32_t psender,
                uint8_t *hwtarget, uint32_t ptarget) {
    uint8_t arppkt[ARP_SIZE];
    struct arphdr *reply = (struct arphdr *) arppkt;
    reply->hwtype = hton16(ARP_HW_TYPE_ENET);
    reply->ptype = hton16(ARP_PTYPE_IPV4);
    reply->hwlen = ARP_HW_ENET_LEN;
    reply->plen = ARP_P_IPV4_LEN;
    reply->opcode = hton16(ARP_OP_REPLY);
    reply->psender = hton32(psender);
    reply->ptarget = hton32(ptarget);
    memcpy(reply->hwsender, hwsender, 6);
    memcpy(reply->hwtarget, hwtarget, 6);
    net_tx(reply->hwtarget, arppkt, sizeof(arppkt), ETHERTYPE_ARP);
}

void arp_deliver(uint8_t *buf) {
    struct arphdr *hdr = (struct arphdr *) buf;
    if (hton32(hdr->ptarget) != ipv4_get_address())
        return;
    if (hton16(hdr->opcode) == ARP_OP_REQUEST)
        arp_send_reply(MAC, ipv4_get_address(), hdr->hwsender, hdr->psender);
}
