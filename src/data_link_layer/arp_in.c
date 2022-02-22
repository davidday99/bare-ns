#include <stdint.h>
#include "arp.h"
#include "ethernet.h"
#include "ipv4.h"
#include "net.h"
#include "netcommon.h"
#include <string.h>

static uint8_t MAC[] = {0xA0, 0xCD, 0xEF, 0x01, 0x23, 0x45};

void arp_deliver(uint8_t *buf) {
    struct arphdr *hdr = (struct arphdr *) buf;
    if (hton32(hdr->ptarget) != ipv4_get_address())
        return;
    if (hton16(hdr->opcode) == ARP_OP_REQUEST)
        arp_send_reply(MAC, ipv4_get_address(), hdr->hwsender, hdr->psender);
}
