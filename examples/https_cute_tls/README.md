# HTTPS cute_tls

This example is perfect for sending and recieving small requests.

An advantage of cute_tls to libcurl is the ease of building and reduced binary size. My release binaries are 22kb on Windows and 55kb on MacOS.

-   Uses `cute_tls.h` for sending requests & recieving raw responses
-   Uses `picohttpparser.h` for reading response headers, status codes and finding the response body.

If you have special needs, such as receiving >1mb of data, or POSTing >1kb of data, then this example code will probably break. You should either modify this example code to suit your needs, or use libcurl instead. Curl is much more robust than this, but you pay for that complexity in various ways. Example code for GET & POST requests using curl can be found in a neighbouring folder to this one.