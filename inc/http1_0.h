#ifndef _HTTP_1_0_H_
#define _HTTP_1_0_H_

#include <stdint.h>

#define MAX_URL_LEN 255

enum http_method {
    GET,
    HEAD,
    POST,
    UPDATE
};


typedef uint16_t http_status_code;

struct http_request_message {
    enum http_method method;
    // header
    char url[MAX_URL_LEN];
    char *body;
};

struct http_response_message {
    http_status_code status_code;
    // header
    char *body;
};

enum http_status_code http_request(char *url, enum http_method method, char *body, struct http_response_message *response);
enum http_status_code http_request_ip(uint32_t ip, enum http_method method, char *body, struct http_response_message *response);
void http_response(http_status_code status_code, char *body);


#endif /* _HTTP_1_0_H_ */