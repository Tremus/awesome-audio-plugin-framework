# HTTPS xrequest

This is largely a copy of the `cute_tls` example found [here](../https_cute_tls/README.md)

The main selling point of `xrequest` is that it's so easy to use that there is no documentation. Why document 1 function that reads well? It's a single header library that's easy to build. It's a tiny library. It doesn't manage memory or threads for you. There is no hidden allocations or global state. It runs on old systems

The Windows implementation is largely the same as `cute_tls`, but the macOS version was rewritten to support older devices than `10.14`. That's the primary reason I wrote it, to support older macs. While I was there I refactored the interface down to 1 function in hopes I would no longer pull my hair out every time I need to do a web request. It's very easy to make a request in JavaScript with `fetch()`, why has no one made anything like this for C or C++?

#### Advantages:

Compared with `cute_tls`:

- `xrequest` supports much older macOS `10.2+`. `cute_tls` requires use of the `Network` Framework, requiring macOS `10.14`. `xrequest` uses TLS 1.2 features in the older `Security` Framework.

- Much easier interface: call 1 function. No need to manage connect/disconnect stuff. No checking of error & data messages in a process loop (this is arguably a disadvantage)

- A couple of edge case bug fixes in the Windows implementation

Compared with `libcurl`:

- Easier interface. No need to RTFM just to make a GET request.
- 0 memory allocations
- Tiny code size
- Trivial to build
- curl is a hot target for attackers. The larger and more complicated a library is, the easier it is to hide something malicious.

#### Disadvantages:

Compared with `cute_tls`:

- If TLS 1.2 ever gets seriously compromised, the connection on macOS will be insecure with `xrequest` (in theory), and will reqruire a reimplementation using the newer `Network` Framework.

- Richer error messages

- Control over connection & disconnection

Compared with `libcurl`:

- Maintainance. If there is ever a security issue with something like the `Security` framework on macOS, you can bet they will be quicker to update their code.
- Rich error messages
- Control over persisting connections and certificates
- Wide community of people who know how to use it and can help you
