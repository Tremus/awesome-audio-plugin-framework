#define CUTE_TLS_IMPLEMENTATION
#include <cute_tls.h>

#include "picohttpparser.h"

int main()
{
    const char* hostname = "www.google.com";
    // const char* hostname = "badssl.com";
    // const char* hostname = "expired.badssl.com";
    // const char* hostname = "wrong.host.badssl.com";
    // const char* hostname = "self-signed.badssl.com";
    // const char* hostname = "untrusted-root.badssl.com";

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
            printf("Error connecting to to %s with code %s.\n", hostname, tls_state_string(state));
            return -1;
        }
    }

    printf("Connected!\n");

    // Send GET request.
    char req[1024];
    int  req_len = sprintf(req, "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", hostname);
    if (tls_send(connection, req, req_len) < 0)
    {
        tls_disconnect(connection);
        printf("Failed to send data.\n");
        return -1;
    }

    size_t bytes_received  = 0;
    size_t bytes_allocated = 0;
    char*  buf             = NULL;
    while (1)
    {
        TLS_State state = tls_process(connection);
        if (state == TLS_STATE_DISCONNECTED)
        {
            break;
        }

        if ((bytes_received + TLS_MAX_PACKET_SIZE) > bytes_allocated)
        {
            bytes_allocated += TLS_MAX_PACKET_SIZE;
            // +1 so we can add a NULL termination
            buf = realloc(buf, bytes_allocated + 1);
        }

        int num_bytes_read = tls_read(connection, buf + bytes_received, sizeof(buf));
        if (num_bytes_read < 0)
        {
            tls_disconnect(connection);
            printf("Failed reading bytes.\n");
            return -1;
        }
        if (num_bytes_read)
        {
            bytes_received += num_bytes_read;
        }
    }
    tls_disconnect(connection);
    buf[bytes_received] = '\0'; // NULL termination

    int total = bytes_received; // in 2023, C still cannot reliably format a size_t on all systems
    printf("Received %d bytes\n", total);

    int               version, status;
    const char*       msg;
    size_t            msg_len;
    size_t            num_headers = 8;
    struct phr_header headers[num_headers];
    phr_parse_response(buf, bytes_received, &version, &status, &msg, &msg_len, headers, &num_headers, 0);

    printf("Message:");
    printf(msg);
    total = msg_len;
    printf("Message len: %d bytes\n", total);

    printf("Version: %d\n", version);
    printf("Status: %d\n", status);
    total = num_headers;
    printf("Num headers: %d\n", total);
    for (size_t i = 0; i < num_headers; i++)
    {
        char text[32];
        snprintf(text, 32, "%s", headers[i].name);
        printf(text);
        snprintf(text, 32, "%s", headers[i].value);
        printf(text);
    }

    return 0;
}