#ifndef _HTTP_1_0_H_
#define _HTTP_1_0_H_

#include <stdint.h>
#include "socket.h"

#define MAX_URI_LEN 255
#define MAX_DATE_LEN 32
#define MAX_PRAGMA_LEN 1
#define MAX_AUTHORIZATION_LEN 1
#define MAX_FROM_LEN 1
#define MAX_IF_MODIFIED_SINCE_LEN 1
#define MAX_REFERER_LEN 1
#define MAX_USER_AGENT_LEN 1

#define MAX_RESPONSE_LEN 1000

enum http_method {
    GET,
    HEAD,
    POST,
    UPDATE,
    NOT_IMPLEMENTED
};

enum http_request_format {
    GOOD_REQUEST,
    BAD_REQUEST
};


typedef uint16_t http_status_code;

struct http_general_header {
    char *date;
    char *pragma;
};

struct http_request_header {
    char *auth;
    char *from;
    char *if_modified_since;
    char *referer;
    char *user_agent;
};

struct http_response_header {
    char *location;
    char *server;
    char *www_auth;
};

struct http_entity_header {
    char *allow;
    char *encoding;
    char *length;
    char *expires;
    char *last_modified;
};

struct http_request_message {
    char version[2];
    struct http_general_header genhdr;
    struct http_request_header reqhdr;
    struct http_entity_header entityhdr;
    enum http_method method;
    char *uri;
    char *body;
    enum http_request_format bad_request;
};

struct http_response_message {
    char version[2];
    http_status_code status_code;
    struct http_general_header genhdr;
    struct http_response_header reshdr;
    struct http_entity_header entityhdr;
    char *body;
};

http_status_code http_request(char *url, enum http_method method, char *body, struct http_response_message *response);
http_status_code http_request_ip(uint32_t ip, enum http_method method, char *body, struct http_response_message *response);
void http_respond(struct socket *sock, http_status_code status_code, char *location, char *body);
uint8_t http_parse_request(uint8_t *buf, struct http_request_message *req);


#endif /* _HTTP_1_0_H_ */