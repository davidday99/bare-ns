#include "icmp.h"
#include <stdint.h>

int test_icmp_calculate_checksum() {
    uint8_t data[] = {
        0, 1, 0xf2, 0x03, 0xf4, 0xf5, 0xf6, 0xf7
    };

    uint16_t result = icmp_calculate_checksum(data, 8);

    return result == 0x0D22;
}

int test_icmp() {
    int success = 1;
    success &= test_icmp_calculate_checksum();
    return success;
}