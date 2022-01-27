#include <stdint.h>

uint16_t ones_complement_sum_buffer(uint8_t *data, uint16_t len) {
    uint16_t *ptr_two_bytes = (uint16_t *) data;
    uint32_t overflow;
    uint32_t sum = 0;
    
    for (uint16_t i = 0; i < len / 2; i++) {
        sum += *ptr_two_bytes++;
    }
    
    if (len & 1) {
        sum += (*ptr_two_bytes & 0xFF) << 8;
    }
    
    while ((overflow = (sum & 0xFFFF0000))) {
        overflow >>= 16;
        sum = (sum & 0xFFFF) + overflow;
    }
    
    return sum;
}

uint16_t ones_complement_sum(uint16_t op1, uint16_t op2) {
    uint32_t overflow;
    uint32_t sum = 0;
    
    sum = op1 + op2;

    while ((overflow = (sum & 0xFFFF0000))) {
        overflow >>= 16;
        sum = (sum & 0xFFFF) + overflow;
    }
    
    return sum;
}