#include <stdint.h>
#include "arp.h"
#include "ethernet.h"
#include "ipv4.h"
#include "net.h"
#include "netcommon.h"
#include <string.h>

void arp_deliver(uint8_t *buf) {
    struct arphdr *hdr = (struct arphdr *) buf;
    if (hton32(hdr->ptarget) == ipv4_get_address()) {
        if (hton16(hdr->opcode) == ARP_OP_REQUEST)
                arp_send_reply(ipv4_get_address(), hdr->hwsender, hton32(hdr->psender));
    }
    if (hton32(hdr->psender) == ipv4_get_default_gateway()) {
        memcpy(GATEWAY_MAC, hdr->hwsender, 6);
    }
}
