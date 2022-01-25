#ifndef _CHECKSUM_H_
#define _CHECKSUM_H_

#include <stdint.h>

uint16_t ones_complement_sum_buffer(uint8_t *data, uint16_t len);
uint16_t ones_complement_sum(uint16_t op1, uint16_t op2);

#endif /* _CHECKSUM_H_ */