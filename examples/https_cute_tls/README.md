# HTTPS cute_tls

-   Uses `cute_tls.h` for sending requests & recieving raw responses
-   Uses `picohttpparser.h` for reading response headers, status codes and finding the response body.

An advantage of cute_tls to libcurl is the ease of building and reduced binary size.

A disadvantage to libcurl is the complexity of sending requests (for now)

TODO: build for Windows
