#ifndef _IPV4_IN_H_
#define _IPV4_IN_H_

#include <stdint.h>
#include "ipv4.h"

void ipv4_deliver(struct ipv4hdr *pkt);

#endif /* _IPV4_IN_H_ */
