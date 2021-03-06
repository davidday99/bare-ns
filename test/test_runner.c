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
int test_socket();
int test_ipv4_to_udp_to_socket();
int test_socket_send();
int test_tcp_checksum();
int test_tcp_recv_syn();
int test_tcp_parse_options();
int test_tcp_socket_send();
int test_http_request_parsing();
int test_http_response_forming();

int main() {
    printf("Running tests...\n\n");
    assert(test_ethernet(), 1, "Failed test_ethernet");
    assert(test_arp(), 1, "Failed test_arp");
    assert(test_ip(), 1, "Failed test_ip");
    assert(test_socket(), 1, "Failed test_socket");
    assert(test_ipv4_to_udp_to_socket(), 1, "Failed test_ipv4_to_udp_to_socket");
    assert(test_socket_send(), 1, "Failed test_socket_send");
    assert(test_tcp_checksum(), 1, "Failed test_tcp_checksum");
    assert(test_tcp_recv_syn(), 1, "Failed test_tcp_recv_syn");
    assert(test_tcp_parse_options(), 1, "Failed test_tcp_parse_options");
    assert(test_tcp_socket_send(), 1, "Failed test_tcp_socket_send");
    assert(test_http_request_parsing(), 1, "Failed test_http_request_parsing");
    assert(test_http_response_forming(), 1, "Failed test_http_response_forming");
    printf("Done.\n");
    return 0;
}