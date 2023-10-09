# HTTPS cURL

Makes simple GET/POST HTTPS requests. In an effort to keep binary sizes small, this only uses libcurl dependencies required for HTTPS.

A single compilation unit is used with all the right defaults for super fast & config free builds. Use curls very own CMakeLists build system at your own peril.

Currently the Release build produces an executable size of ~432kb on MacOS, ~372 on Windows. On MacOS you will double that size if you target arm64 & x86_64.

If you change any settings and produce a smaller build, please let me know!

NOTE: If you build libcurl as a static library, you must have `CURL_STATICLIB` defined before including `curl/curl.h`. If not, do not define `CURL_STATICLIB`.