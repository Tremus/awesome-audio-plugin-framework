#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define CUTE_TLS_IMPLEMENTATION
#include <cute_tls.h>

#include "picohttpparser.h"

struct https_response
{
    char*    buffer;
    uint64_t size;
    uint64_t capacity;

    int status_code;
    int body_offset; // body = buffer + body_offset

    // Array of string views
    size_t             num_headers;
    struct phr_header* headers;
};

void https_free(struct https_response* res)
{
    if (res->buffer != NULL)
        free(res->buffer);
}

void https_read_response(struct https_response* res, TLS_Connection* conn)
{
    res->capacity = TLS_MAX_RECORD_SIZE * 2;
    res->buffer   = malloc(res->capacity + TLS_1_KB);
    while (1)
    {
        if (tls_process(*conn) == TLS_STATE_DISCONNECTED)
            break;

        if ((res->size + TLS_MAX_RECORD_SIZE) >= res->capacity)
        {
            size_t grow_amt = res->capacity > TLS_MAX_RECORD_SIZE ? res->capacity : TLS_MAX_RECORD_SIZE;
            res->capacity   += grow_amt;
            res->buffer     = realloc(res->buffer, res->capacity + TLS_1_KB);
        }

        int num_bytes_read = tls_read(*conn, res->buffer + res->size, TLS_MAX_PACKET_SIZE);
        if (num_bytes_read < 0)
        {
            tls_disconnect(*conn);
            fprintf(stderr, "Failed reading bytes.\n");
            return;
        }
        res->size += num_bytes_read;
    }
    tls_disconnect(*conn);
    // NULL terminate our response body. Makes it more compatible with C
    // functions
    res->buffer[res->size++] = '\0';
    res->capacity            += TLS_1_KB;

    int         version;
    const char* msg;
    size_t      msg_len;

    // If we don't allocate enough headers, phr_parse_response won't return the
    // offset to our response body We already have at least 1kb padded memory
    // allocated in our response buffer Use remaining memory to store headers
    // (at least 1kb / 32bytes = 32 headers)
    res->num_headers = (res->capacity - res->size) / sizeof(struct phr_header);
    res->headers     = (struct phr_header*)(res->buffer + res->size + (res->size % 8));

    res->body_offset = phr_parse_response(
        res->buffer,
        res->size,
        &version,
        &res->status_code,
        &msg,
        &msg_len,
        res->headers,
        &res->num_headers,
        0);
}

struct https_response https_get(const char* hostname, const char* pathname)
{
    struct https_response res;
    memset(&res, 0, sizeof(res));

    TLS_Connection connection = tls_connect(hostname, 443);

    while (1)
    {
        TLS_State state = tls_process(connection);
        if (state == TLS_STATE_CONNECTED)
        {
            break;
        }
        else if (state < 0)
        {
            fprintf(stderr, "Error connecting to to %s with code %s.\n", hostname, tls_state_string(state));
            return res;
        }
    }

    // Send GET request.
    char req[1024];
    int  req_len = sprintf(
        req,
        "GET %s HTTP/1.1\r\n"
         "Host: %s\r\n"
         "Connection: close\r\n\r\n",
        pathname,
        hostname);
    if (tls_send(connection, req, req_len) < 0)
    {
        tls_disconnect(connection);
        fprintf(stderr, "Failed to send data.\n");
        return res;
    }

    https_read_response(&res, &connection);
    return res;
}

struct https_response https_post(const char* hostname, const char* path, const char* content_type, const char* data)
{
    struct https_response res;
    memset(&res, 0, sizeof(res));

    TLS_Connection connection = tls_connect(hostname, 443);

    while (1)
    {
        TLS_State state = tls_process(connection);
        if (state == TLS_STATE_CONNECTED)
        {
            break;
        }
        else if (state < 0)
        {
            fprintf(stderr, "Error connecting to to %s with code %s.\n", hostname, tls_state_string(state));
            return res;
        }
    }

    char req[1024];
    int  req_len = sprintf(
        req,
        "POST %s HTTP/1.1\r\n"
         "Host: %s\r\n"
         "Connection: close\r\n"
         "Accept: */*\r\n"
         "Content-Type: %s\r\n"
         "Content-Length:%d\r\n"
         "\r\n"
         "%s",
        path,
        hostname,
        content_type,
        (int)strlen(data),
        data);
    assert(req_len < 1024);

    if (tls_send(connection, req, req_len) < 0)
    {
        tls_disconnect(connection);
        fprintf(stderr, "Failed to send data.\n");
        return res;
    }

    https_read_response(&res, &connection);
    return res;
}

void print_response(struct https_response* res)
{
    fprintf(stderr, "Bytes received %llu\n", res->size);
    fprintf(stderr, "Bytes remaining %llu\n", (res->capacity - res->size));
    fprintf(stderr, "Status code: %d\n", res->status_code);
    fprintf(stderr, "Num headers: %d\n", (int)res->num_headers);
    for (size_t i = 0; i < res->num_headers; i++)
    {
        struct phr_header* h = &res->headers[i];

        fprintf(stderr, "%.*s: %.*s\n", (int)(h->name_len), h->name, (int)h->value_len, h->value);
    }

    fprintf(stderr, "Response body\n");
    if (res->body_offset > 0)
        fprintf(stderr, "%.*s\n", (int)(res->size - res->body_offset), res->buffer + res->body_offset);
}

int main()
{
    const char* get_hostname = "httpbin.org";
    const char* get_pathname = "/get";

    fprintf(stderr, "GET - %s%s\n", get_hostname, get_pathname);
    struct https_response res = https_get(get_hostname, get_pathname);

    if (res.status_code == 0)
    {
        fprintf(stderr, "Failed GET\n");
        return -1;
    }
    print_response(&res);
    https_free(&res);

    const char* post_hostname = "httpbin.org";
    const char* post_pathname = "/post";
    const char* content_type  = "application/json";
    const char* data          = "{\"message\": \"hello\"}";

    fprintf(stderr, "POST - %s%s\n", post_hostname, post_pathname);
    res = https_post(post_hostname, post_pathname, content_type, data);

    if (res.status_code == 0)
    {
        fprintf(stderr, "Failed POST\n");
        return -1;
    }
    print_response(&res);
    https_free(&res);

    return 0;
}