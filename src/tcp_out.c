#include "tcp.h"
#include "ipv4.h"
#include "netcommon.h"
#include "string.h"
#include "checksum.h"

void tcp_send(uint32_t destip, uint8_t *data, uint16_t len) {
    uint8_t pseudohdr[sizeof(struct pseudohdr)];
    struct pseudohdr *phdr = (struct pseudohdr *) pseudohdr;
    struct tcphdr *hdr = (struct tcphdr *) data;

    phdr->srcip = hton32(ipv4_get_address());
    phdr->destip = hton32(destip);
    phdr->zero = 0;
    phdr->pctl = 6;
    phdr->len = hton16(len);
    hdr->cksm = calculate_tcp_checksum(data, pseudohdr);
    

    ipv4_send(destip, data, len, IPV4_PROTOCOL_TCP);
}