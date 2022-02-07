#include "http1_0.h"
#include "string.h"

static char http_message[] = "GET / HTTP/1.0\r\n";

int test_http_request_parsing() {
    int success = 1;

    struct http_request_message req;

    http_parse_request(http_message, &req);

    success &= req.method == GET;
    success &= strcmp(req.uri, "/") == 0;
    success &= req.version[0] == '1' && req.version[1] == '0';
    return success;
}