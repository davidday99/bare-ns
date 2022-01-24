#ifndef _CHECKSUM_H_
#define _CHECKSUM_H_

#include <stdint.h>

uint16_t ones_complement_sum(uint8_t *data, uint16_t len);

#endif /* _CHECKSUM_H_ */