#include "icmp.h"

static uint16_t icmp_sum_ones_complement(uint8_t *data, uint16_t len);

void icmp_request(struct icmp_echohdr *hdr) {
    hdr->type = ICMP_TYPE_ECHO;
    hdr->code = 0;
    return;
}

void icmp_reply(struct icmp_echohdr *hdr) {
    return;
}

uint16_t icmp_calculate_checksum(uint8_t *data, uint16_t len) {
    uint16_t sum = icmp_sum_ones_complement(data, len);
    return ~sum;
}

static uint16_t icmp_sum_ones_complement(uint8_t *data, uint16_t len) {
    uint16_t *hwordptr = (uint16_t *) data;
    uint32_t chksm = 0;
    uint8_t odd = len & 1;
    for (uint16_t i = 0; i < (len - odd) / 2; i++) {
        chksm += *hwordptr++;
    }
    if (odd) {
        chksm += (uint8_t) *hwordptr << 8;
    }
    chksm += (chksm >> 16) & 0xFFFF;
    return (uint16_t) chksm;
}