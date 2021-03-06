#include "http1_0.h"
#include "socket.h"

int test_http_response_forming() {
    int success = 1;

    struct socket *s = socket_init(SOCKTYPE_TCP);

    http_respond(s, 200, "192.168.0.111", "hello world");

    http_respond(s, 202, "192.168.0.111", "testing");

    http_respond(s, 204, "192.168.0.111", 0);

    return success;
}