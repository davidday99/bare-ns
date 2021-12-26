#ifndef _ICMP_H_
#define _ICMP_H_

#include <stdint.h>

#define ICMP_TYPE_ECHO_REPLY 0
#define ICMP_TYPE_DEST_UNREACHABLE 3
#define ICMP_TYPE_ECHO 8

#define ICMP_ECHO_DATA_OFFSET 8

struct icmp_echohdr {
    uint8_t type;
    uint8_t code;
    uint16_t chksm;
    uint16_t id;
    uint16_t seqnum;
};

void icmp_request(struct icmp_echohdr *hdr);
void icmp_reply(struct icmp_echohdr *hdr);
uint16_t icmp_calculate_checksum(uint8_t *data, uint16_t len);

#endif /* _ICMP_H_ */