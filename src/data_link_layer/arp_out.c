#include <stdint.h>
#include "arp.h"
#include "ethernet.h"
#include "ipv4.h"
#include "net.h"
#include "netcommon.h"
#include <string.h>

void arp_send_request(uint32_t psender, uint32_t ptarget) {
    uint8_t arppkt[ARP_SIZE];
    struct arphdr *request = (struct arphdr *) arppkt;
    uint8_t mac[6];
    net_mac_address(mac);
    request->hwtype = hton16(ARP_HW_TYPE_ENET);
    request->ptype = hton16(ARP_PTYPE_IPV4);
    request->hwlen = ARP_HW_ENET_LEN;
    request->plen = ARP_P_IPV4_LEN;
    request->opcode = hton16(ARP_OP_REQUEST);
    request->psender = hton32(psender);
    request->ptarget = hton32(ptarget);
    memcpy(request->hwsender, mac, 6);
    memcpy(request->hwtarget, MAC_BROADCAST, 6);
    net_tx(MAC_BROADCAST, arppkt, sizeof(arppkt), ETHERTYPE_ARP);
}

void arp_send_reply(uint32_t psender, uint8_t *hwtarget, uint32_t ptarget) {
    uint8_t arppkt[ARP_SIZE];
    struct arphdr *reply = (struct arphdr *) arppkt;
    uint8_t mac[6];
    net_mac_address(mac);
    reply->hwtype = hton16(ARP_HW_TYPE_ENET);
    reply->ptype = hton16(ARP_PTYPE_IPV4);
    reply->hwlen = ARP_HW_ENET_LEN;
    reply->plen = ARP_P_IPV4_LEN;
    reply->opcode = hton16(ARP_OP_REPLY);
    reply->psender = hton32(psender);
    reply->ptarget = hton32(ptarget);
    memcpy(reply->hwsender, mac, 6);
    memcpy(reply->hwtarget, hwtarget, 6);
    net_tx(reply->hwtarget, arppkt, sizeof(arppkt), ETHERTYPE_ARP);
}
