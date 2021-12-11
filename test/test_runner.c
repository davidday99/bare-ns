#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t assert(uint32_t expected, uint32_t actual, char *message) {
    if (expected != actual) {
        printf("%s\n", message);
    }
}

int test_ethernet();
int test_arp();
int test_ethernet_arp();

int main() {
    printf("Running tests...\n\n");
    assert(test_ethernet(), 1, "Failed test_ethernet()");
    assert(test_arp(), 1, "Failed test_arp()");
    assert(test_ethernet_arp(), 1, "Failed test_ethernet_arp()");
    printf("Done.\n");
    return 0;
}