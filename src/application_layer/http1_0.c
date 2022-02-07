#include "http1_0.h"
#include <stdint.h>
#include "string.h"

#define DELIMITER "\r\n"

static uint8_t *parse_request_line(uint8_t *buf, struct http_request_message *req);
static uint8_t *parse_request_method(uint8_t *buf, struct http_request_message *req);
static uint8_t *parse_request_uri(uint8_t *buf, struct http_request_message *req);
static uint8_t *parse_version(uint8_t *buf, struct http_request_message *req);

static uint8_t *set_status_line(uint8_t *buf, struct http_response_message *res);
static uint8_t *set_response_headers(uint8_t *buf, char *location);
static uint8_t *set_entity_headers(uint8_t *buf, struct http_response_message *res);
static uint8_t *delimit_headers(uint8_t *buf);
static uint8_t *append_entity_body(uint8_t *buf, char *body);
const char *get_reason_phrase(http_status_code status_code);

uint8_t itos(uint32_t val, char *buf);

http_status_code http_request(char *url, enum http_method method, char *body, struct http_response_message *response) {
    return (http_status_code) 200;
}

http_status_code http_request_ip(uint32_t ip, enum http_method method, char *body, struct http_response_message *response) {
    return (http_status_code) 200;
}

void http_respond(struct socket *s, http_status_code status_code, char *location, char *body) {
    uint8_t response[MAX_RESPONSE_LEN];
    uint8_t *bufptr = response;
    struct http_response_message res;
    res.version[0] = '1';
    res.version[1] = '0';
    res.status_code = status_code;
    res.body = body;
    
    bufptr = set_status_line(response, &res);

    // set general headers -- ignore for now

    // set response headers
    bufptr = set_response_headers(bufptr, location);

    // set entity headers
    bufptr = set_entity_headers(bufptr, &res);

    bufptr = delimit_headers(bufptr);

    // append entity body, if any
    if (body != 0)
        bufptr = append_entity_body(bufptr, body);

    uint16_t response_len = bufptr - response;

    socket_send(s, response, response_len);
}

static uint8_t *set_status_line(uint8_t *buf, struct http_response_message *res) {
    char sc_string[4];
    itos(res->status_code, sc_string);
    const char *reason_phrase = get_reason_phrase(res->status_code);

    memcpy(buf, "HTTP/", 5);
    buf += 5;
    *buf++ = res->version[0];
    *buf++ = '.';
    *buf++ = res->version[1];
    *buf++ = ' ';

    memcpy(buf, sc_string, 3);
    buf += 3;
    *buf++ = ' ';

    strcpy(buf, reason_phrase);
    buf += strlen(reason_phrase);
    *buf++ = '\r';
    *buf++ = '\n';

    return buf;
}

static uint8_t *set_response_headers(uint8_t *buf, char *location) {
    memcpy(buf, "Location: ", 10);
    buf += 10;
    strcpy(buf, location);
    buf += strlen(location);
    *buf++ = '\r';
    *buf++ = '\n';
    return buf;
}

static uint8_t *set_entity_headers(uint8_t *buf, struct http_response_message *res) {
    uint16_t body_len;
    char len_string[6];

    if (res->body != 0)
        body_len = strlen(res->body);
    else
        body_len = 0;

    itos(body_len, len_string);

    memcpy(buf, "Content-Type: text/html\r\n", 25);
    buf += 25;
    memcpy(buf, "Content-Length: ", 16);
    buf += 16;
    strcpy(buf, len_string);
    buf += strlen(len_string);
    *buf++ = '\r';
    *buf++ = '\n';
    return buf;
}

static uint8_t *delimit_headers(uint8_t *buf) {
    *buf++ = '\r';
    *buf++ = '\n';
    return buf;
}

static uint8_t *append_entity_body(uint8_t *buf, char *body) {
    uint16_t body_len = strlen(body);
    memcpy(buf, body, body_len);
    buf += body_len;
    return buf;
}

// TODO: handle malformed requests
uint8_t http_parse_request(uint8_t *buf, struct http_request_message *req) {
    uint8_t *bufptr = buf;
    bufptr = parse_request_line(buf, req);
    // parse general headers
    // parse request headers
    // parse entity headers

    return 1;  // return 1 if valid request, else return 0
}

static uint8_t *parse_request_line(uint8_t *buf, struct http_request_message *req) {
    uint8_t *bufptr = buf;
    bufptr = parse_request_method(bufptr, req);
    bufptr = parse_request_uri(bufptr, req);
    bufptr = parse_version(bufptr, req);

    while (memcmp(bufptr, DELIMITER, 2) != 0)
        bufptr++;

    return bufptr;
}

static uint8_t *parse_request_method(uint8_t *buf, struct http_request_message *req) {
    char method[5];
    uint8_t i = 0;

    while (*buf == ' ' && *buf == '\t') {
        *buf++;
    }
    while (*buf != ' ' && *buf != '\t') {
        method[i++] = *buf++;
    }
    *buf++ = '\0';
    method[i] = '\0';

    if (strcmp(method, "GET") == 0) {
        req->method = GET;
    } else if (strcmp(method, "HEAD") == 0) {
        req->method = HEAD;
    } else if (strcmp(method, "POST") == 0) {
        req->method = POST;
    } else if (strcmp(method, "UPDATE") == 0) {
        req->method = UPDATE;
    }

    return buf;
}

static uint8_t *parse_request_uri(uint8_t *buf, struct http_request_message *req) {
    while (*buf == ' ' && *buf == '\t') {
        *buf++;
    }
    req->uri = buf;
    while (*buf != ' ' && *buf != '\t') {
        *buf++;
    }
    *buf++ = '\0';

    return buf;
}

static uint8_t *parse_version(uint8_t *buf, struct http_request_message *req) {
    while (*buf == ' ' && *buf == '\t') {
        *buf++;
    }

    while (*buf++ != '/')
        ;

    req->version[0] = *buf;
    buf += 2;  // skip the decimal
    req->version[1] = *buf++;

    return buf;
}

const char *get_reason_phrase(http_status_code status_code) {
    if (status_code == 200)
        return "OK";
    if (status_code == 202)
        return "ACCEPTED";
    if (status_code == 204)
        return "NO CONTENT";
}