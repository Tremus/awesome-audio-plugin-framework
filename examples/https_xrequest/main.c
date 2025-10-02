#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define XHL_REQUEST_IMPL
#include <xrequest.h>

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

bool g_should_cancel = false;

int my_cb_xreq(
    void*       user,
    const void* data, // May be NULL
    unsigned    size  // May be zero
)
{
    struct https_response* res = (struct https_response*)user;

    if (g_should_cancel)
        return 1;

    if (data != NULL && size > 0)
    {
        size_t required_cap = res->size + size;
        if (required_cap > res->capacity)
        {
            size_t next_cap = required_cap * 2;
            res->buffer     = realloc(res->buffer, next_cap);
            res->capacity   = next_cap;
        }

        memcpy(res->buffer + res->size, data, size);
        res->size += size;
    }

    return 0;
}

void parse_response_headers(struct https_response* res)
{
    static struct phr_header LOTS_OF_HEADERS[420];

    // Use picohttpparser (phr) to parse headers
    // phr is a bit dumb. It doesn't check the capacity of the header array you pass it. In theory this could produce a
    // buffer overrun, but in practice if you hand it an unreasonably large buffer, it won't happen.
    res->headers     = LOTS_OF_HEADERS;
    res->num_headers = sizeof(LOTS_OF_HEADERS) / sizeof(LOTS_OF_HEADERS[0]);
    int         version;
    const char* msg;
    size_t      msg_len;
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
    // Probably more than enough memory. YOLO
    static char* req_buffer[1024 * 16] = {0};

    // GET
    {
        const char* get_hostname = "httpbin.org";
        const char* get_pathname = "/get";

        // clang-format off
        int req_len = snprintf(
            (char*)req_buffer,
            sizeof(req_buffer),
            "GET %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n"
            "\r\n",
            get_pathname,
            get_hostname);
        // clang-format on
        fprintf(stderr, "Preparing this GET request:\n\n");
        fwrite(req_buffer, 1, req_len, stderr);

        // Blocking call
        struct https_response res = {0};
        xrequest(get_hostname, 443, (const char*)req_buffer, req_len, &res, my_cb_xreq);

        parse_response_headers(&res);

        if (res.status_code == 0)
        {
            fprintf(stderr, "Failed GET\n");
            return -1;
        }
        print_response(&res);

        free(res.buffer);
    }

    // POST
    {
        const char* post_hostname = "httpbin.org";
        const char* post_pathname = "/post";
        const char* content_type  = "application/json";
        const char* data          = "{\"message\": \"hello\"}";

        size_t datalen = strlen(data);

        // clang-format off
        int req_len = snprintf(
            (char*)req_buffer,
            sizeof(req_buffer),
            "POST %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n"
            "Accept: */*\r\n"
            "Content-Type: %s\r\n"
            "Content-Length:%zu\r\n"
            "\r\n"
            "%s",
            post_pathname,
            post_hostname,
            content_type,
            datalen,
            data
        );
        // clang-format on
        fprintf(stderr, "Preparing this POST request:\n\n");
        fwrite(req_buffer, 1, req_len, stderr);

        // Blocking call
        struct https_response res = {0};
        xrequest(post_hostname, 443, (const char*)req_buffer, req_len, &res, my_cb_xreq);

        parse_response_headers(&res);

        if (res.status_code == 0)
        {
            fprintf(stderr, "Failed POST\n");
            return -1;
        }
        print_response(&res);

        free(res.buffer);
    }

    // Cancel request
    {
        const char* get_hostname = "httpbin.org";
        const char* get_pathname = "/get";

        // clang-format off
        int req_len = snprintf(
            (char*)req_buffer,
            sizeof(req_buffer),
            "GET %s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n"
            "\r\n",
            get_pathname,
            get_hostname);
        // clang-format on
        fprintf(stderr, "Preparing this GET request:\n\n");
        fwrite(req_buffer, 1, req_len, stderr);

        // Blocking call
        fprintf(stderr, "Testing early exit out of xrequest()\n");
        // This flag is read by our callback function
        g_should_cancel           = true;
        struct https_response res = {0};
        xrequest(get_hostname, 443, (const char*)req_buffer, req_len, &res, my_cb_xreq);

        fprintf(stderr, "Response size: %zu\n", res.size);

        if (res.buffer || res.size || res.capacity)
        {
            fprintf(stderr, "Failed to cancel request!\n");
            return -1;
        }
        fprintf(stderr, "Successfully exited xrequest() early\n");
    }

    fprintf(stderr, "Done!\n");
    return 0;
}