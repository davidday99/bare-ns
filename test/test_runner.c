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
int test_ip();
int test_icmp();
int test_socket();

int main() {
    printf("Running tests...\n\n");
    assert(test_ethernet(), 1, "Failed test_ethernet");
    assert(test_arp(), 1, "Failed test_arp");
    assert(test_ip(), 1, "Failed test_ip");
    assert(test_icmp(), 1, "Failed test_icmp");
    assert(test_socket(), 1, "Failed test_socket");
    printf("Done.\n");
    return 0;
}