// Here's a (hopefully) pain free single compilation unit for libcurl
// This is meant to be an alternative to building through CMake, which
// requires an arduous configuration step

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#define BUILDING_LIBCURL
// Remove CURL_STATICLIB if you're using libcurl_scu.c inside another scu
#define CURL_STATICLIB

// lib/curl_config.h

// #undef CURL_DEFAULT_SSL_BACKEND

#define CURL_DISABLE_ALTSVC 1
#define CURL_DISABLE_COOKIES 1
#define CURL_DISABLE_DICT 1
#define CURL_DISABLE_FILE 1
#define CURL_DISABLE_FORM_API 1
#define CURL_DISABLE_FTP 1
#define CURL_DISABLE_GOPHER 1
#define CURL_DISABLE_IMAP 1
#define CURL_DISABLE_LDAP 1
#define CURL_DISABLE_LDAPS 1
#define CURL_DISABLE_LIBCURL_OPTION 1
#define CURL_DISABLE_MIME 1
#define CURL_DISABLE_MQTT 1
#define CURL_DISABLE_NETRC 1
#define CURL_DISABLE_NTLM 1
#define CURL_DISABLE_PARSEDATE 1
#define CURL_DISABLE_POP3 1
#define CURL_DISABLE_PROGRESS_METER 1
#define CURL_DISABLE_PROXY 1
#define CURL_DISABLE_RTSP 1
#define CURL_DISABLE_SMB 1
#define CURL_DISABLE_SMTP 1
#define CURL_DISABLE_SOCKETPAIR 1
#define CURL_DISABLE_TELNET 1
#define CURL_DISABLE_TFTP 1
#define CURL_DISABLE_VERBOSE_STRINGS 1
#define CURL_EXTERN_SYMBOL __attribute__((__visibility__("default")))

#define HAVE_ATOMIC 1
#define HAVE_BOOL_T 1
#define HAVE_BUILTIN_AVAILABLE 1
#define HAVE_FCNTL_H 1
#define HAVE_FREEADDRINFO 1
#define HAVE_GETADDRINFO 1
#define HAVE_GETHOSTNAME 1
#define HAVE_GETPEERNAME 1
#define HAVE_GETSOCKNAME 1
#define HAVE_INET_NTOP 1
#define HAVE_INET_PTON 1
#define HAVE_INTTYPES_H 1
#define HAVE_LOCALE_H 1
#define HAVE_LONGLONG 1
#define HAVE_RECV 1
#define HAVE_SELECT 1
#define HAVE_SEND 1
#define HAVE_SETJMP_H 1
#define HAVE_SETLOCALE 1
#define HAVE_SETMODE 1
#define HAVE_SIGNAL 1
#define HAVE_SIGNAL_H 1
#define HAVE_SOCKET 1
#define HAVE_STDATOMIC_H 1
#define HAVE_STDBOOL_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRDUP 1
#define HAVE_STRINGS_H 1
#define HAVE_STRING_H 1
#define HAVE_STRTOLL 1
#define HAVE_STRUCT_SOCKADDR_STORAGE 1
#define HAVE_STRUCT_TIMEVAL 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_TIME_H 1
#define HAVE_UTIME 1
#define HAVE_VARIADIC_MACROS_C99 1
#define HAVE_VARIADIC_MACROS_GCC 1
#define SIZEOF_INT 4
#define SIZEOF_LONG_LONG 8
#define SIZEOF_CURL_OFF_T 8
#define SIZEOF_SIZE_T 8
#define SIZEOF_TIME_T 8
#define STDC_HEADERS 1
#define CURL_DISABLE_OPENSSL_AUTO_LOAD_CONFIG 1
#define USE_UNIX_SOCKETS
#ifndef _ALL_SOURCE
#undef _ALL_SOURCE
#endif
#define _FILE_OFFSET_BITS 64
#ifndef __cplusplus
#undef inline
#endif
#define HAVE_MACH_ABSOLUTE_TIME 1

#ifdef _WIN32

// #define USE_WIN32_CRYPTO 1
#define HAVE_CLOSESOCKET 1
#define HAVE_IOCTLSOCKET 1
#define HAVE_IOCTLSOCKET_FIONBIO 1
#define HAVE_IO_H 1
#define HAVE_IDN2_H 1
#define HAVE_SOCKADDR_IN6_SIN6_SCOPE_ID 1
#define HAVE_STRICMP 1
#define HAVE_SYS_UTIME_H 1
#define HAVE_WINDOWS_H 1
#define HAVE_WINSOCK2_H 1
#define HAVE_WS2TCPIP_H 1
#define OS "Windows"
#define SIZEOF_LONG 4
#define SIZEOF_OFF_T 4
// #define USE_WIN32_LARGE_FILES 1
#define USE_WINDOWS_SSPI 1
#define USE_SCHANNEL 1
#define in_addr_t unsigned long
#define ssize_t __int64

#elif defined(__APPLE__)

#define HAVE_SNPRINTF
#define CURL_CA_BUNDLE "/etc/ssl/cert.pem"
#define CURL_CA_PATH "/etc/ssl/certs"
#define HAVE_ALARM 1
#define HAVE_ARPA_INET_H 1
#define HAVE_FCHMOD 1
#define HAVE_BASENAME 1
#define HAVE_CLOCK_GETTIME_MONOTONIC 1
#define HAVE_FCNTL_O_NONBLOCK 1
#define HAVE_FTRUNCATE 1
#define HAVE_GETEUID 1
#define HAVE_GETPPID 1
#define HAVE_IF_NAMETOINDEX 1
#define HAVE_GETPWUID 1
#define HAVE_GETPWUID_R 1
#define HAVE_GETRLIMIT 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_GMTIME_R 1
#define HAVE_IFADDRS_H 1
#define HAVE_SA_FAMILY_T 1
#define HAVE_IOCTL_FIONBIO 1
#define HAVE_IOCTL_SIOCGIFADDR 1
#define HAVE_LDAP_URL_PARSE 1
#define HAVE_LIBGEN_H 1
#define HAVE_MSG_NOSIGNAL 1
#define HAVE_NETDB_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_NETINET_TCP_H 1
#define HAVE_NET_IF_H 1
#define HAVE_PIPE 1
#define HAVE_POLL_H 1
#define HAVE_POSIX_STRERROR_R 1
#define HAVE_PWD_H 1
#define HAVE_FSETXATTR 1
#define HAVE_FSETXATTR_6 1
#define HAVE_SETRLIMIT 1
#define HAVE_SIGACTION 1
#define HAVE_SIGINTERRUPT 1
#define HAVE_SIGSETJMP 1
#define HAVE_SOCKETPAIR 1
#define HAVE_STRCASECMP 1
#define HAVE_STRERROR_R 1
#define HAVE_STRTOK_R 1
#define HAVE_SYS_FILIO_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_POLL_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_SOCKIO_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_UN_H 1
#define HAVE_TERMIOS_H 1
#define HAVE_UNISTD_H 1
#define HAVE_UTIMES 1
#define HAVE_UTIME_H 1
#define HAVE_LIBZ 1
#define OS "Darwin"
#define RANDOM_FILE "/dev/urandom"
#define SIZEOF_LONG 8
#define SIZEOF_OFF_T 8
#define TIME_WITH_SYS_TIME 1
#define USE_SECTRANSP 1

#endif

#include <curl/curl.h>

#include <curl/../../lib/altsvc.c>
#include <curl/../../lib/amigaos.c>
#include <curl/../../lib/asyn-ares.c>
#include <curl/../../lib/asyn-thread.c>
#include <curl/../../lib/base64.c>
#include <curl/../../lib/bufq.c>
#include <curl/../../lib/bufref.c>
#include <curl/../../lib/c-hyper.c>
#include <curl/../../lib/cf-h1-proxy.c>
#include <curl/../../lib/cf-h2-proxy.c>
#include <curl/../../lib/cf-haproxy.c>
#include <curl/../../lib/cf-https-connect.c>
#include <curl/../../lib/cf-socket.c>
#include <curl/../../lib/cfilters.c>
#include <curl/../../lib/conncache.c>
#include <curl/../../lib/connect.c>
#include <curl/../../lib/content_encoding.c>
#include <curl/../../lib/cookie.c>
#include <curl/../../lib/curl_addrinfo.c>
#include <curl/../../lib/curl_des.c>
#include <curl/../../lib/curl_endian.c>
#include <curl/../../lib/curl_fnmatch.c>
#include <curl/../../lib/curl_get_line.c>
#include <curl/../../lib/curl_gethostname.c>
#include <curl/../../lib/curl_gssapi.c>
#include <curl/../../lib/curl_memrchr.c>
#include <curl/../../lib/curl_multibyte.c>
#include <curl/../../lib/curl_ntlm_core.c>
#include <curl/../../lib/curl_ntlm_wb.c>
#include <curl/../../lib/curl_path.c>
#include <curl/../../lib/curl_range.c>
#include <curl/../../lib/curl_rtmp.c>
#include <curl/../../lib/curl_sasl.c>
#include <curl/../../lib/curl_sspi.c>
#include <curl/../../lib/curl_threads.c>
#include <curl/../../lib/curl_trc.c>
#include <curl/../../lib/dict.c>
#include <curl/../../lib/doh.c>
#include <curl/../../lib/dynbuf.c>
#include <curl/../../lib/dynhds.c>
#include <curl/../../lib/easy.c>
#include <curl/../../lib/easygetopt.c>
#include <curl/../../lib/easyoptions.c>
#include <curl/../../lib/escape.c>
#include <curl/../../lib/file.c>
#include <curl/../../lib/fileinfo.c>
#include <curl/../../lib/fopen.c>
#include <curl/../../lib/formdata.c>
#include <curl/../../lib/ftp.c>
#include <curl/../../lib/ftplistparser.c>
#include <curl/../../lib/getenv.c>
#include <curl/../../lib/getinfo.c>
#include <curl/../../lib/gopher.c>
#include <curl/../../lib/hash.c>
#include <curl/../../lib/headers.c>
#include <curl/../../lib/hmac.c>
#include <curl/../../lib/hostasyn.c>
#include <curl/../../lib/hostip.c>
#include <curl/../../lib/hostip4.c>
#include <curl/../../lib/hostip6.c>
#include <curl/../../lib/hostsyn.c>
#include <curl/../../lib/hsts.c>
#include <curl/../../lib/http.c>
#include <curl/../../lib/http1.c>
#include <curl/../../lib/http2.c>
#include <curl/../../lib/http_aws_sigv4.c>
#include <curl/../../lib/http_chunks.c>
#include <curl/../../lib/http_digest.c>
#include <curl/../../lib/http_negotiate.c>
#include <curl/../../lib/http_ntlm.c>
#include <curl/../../lib/http_proxy.c>
#include <curl/../../lib/idn.c>
#include <curl/../../lib/if2ip.c>
#include <curl/../../lib/imap.c>
#include <curl/../../lib/inet_ntop.c>
#include <curl/../../lib/inet_pton.c>
#include <curl/../../lib/krb5.c>
#include <curl/../../lib/ldap.c>
#include <curl/../../lib/llist.c>
#include <curl/../../lib/macos.c>
#include <curl/../../lib/md4.c>
#include <curl/../../lib/md5.c>
#include <curl/../../lib/memdebug.c>
#include <curl/../../lib/mime.c>
#include <curl/../../lib/mprintf.c>
#include <curl/../../lib/mqtt.c>
#include <curl/../../lib/multi.c>
#include <curl/../../lib/netrc.c>
#include <curl/../../lib/nonblock.c>
#include <curl/../../lib/noproxy.c>
#include <curl/../../lib/openldap.c>
#include <curl/../../lib/parsedate.c>
#include <curl/../../lib/pingpong.c>
#include <curl/../../lib/pop3.c>
#include <curl/../../lib/progress.c>
#include <curl/../../lib/psl.c>
#include <curl/../../lib/rand.c>
#include <curl/../../lib/rename.c>
#include <curl/../../lib/rtsp.c>
#include <curl/../../lib/select.c>
#include <curl/../../lib/sendf.c>
#include <curl/../../lib/setopt.c>
#include <curl/../../lib/sha256.c>
#include <curl/../../lib/share.c>
#include <curl/../../lib/slist.c>
#include <curl/../../lib/smb.c>
#include <curl/../../lib/smtp.c>
#include <curl/../../lib/socketpair.c>
#include <curl/../../lib/socks.c>
#include <curl/../../lib/socks_gssapi.c>
#include <curl/../../lib/socks_sspi.c>
#include <curl/../../lib/speedcheck.c>
#include <curl/../../lib/splay.c>
#include <curl/../../lib/strcase.c>
#include <curl/../../lib/strdup.c>
#include <curl/../../lib/strerror.c>
#include <curl/../../lib/strtok.c>
#include <curl/../../lib/strtoofft.c>
#include <curl/../../lib/system_win32.c>
#include <curl/../../lib/telnet.c>
#include <curl/../../lib/tftp.c>
#include <curl/../../lib/timediff.c>
#include <curl/../../lib/timeval.c>
#include <curl/../../lib/transfer.c>
#include <curl/../../lib/url.c>
#include <curl/../../lib/urlapi.c>
#include <curl/../../lib/vauth/cleartext.c>
#include <curl/../../lib/vauth/cram.c>
#include <curl/../../lib/vauth/digest.c>
#include <curl/../../lib/vauth/digest_sspi.c>
#include <curl/../../lib/vauth/gsasl.c>
#include <curl/../../lib/vauth/krb5_gssapi.c>
#include <curl/../../lib/vauth/krb5_sspi.c>
#include <curl/../../lib/vauth/ntlm.c>
#include <curl/../../lib/vauth/ntlm_sspi.c>
#include <curl/../../lib/vauth/oauth2.c>
#include <curl/../../lib/vauth/spnego_gssapi.c>
#include <curl/../../lib/vauth/spnego_sspi.c>
#include <curl/../../lib/vauth/vauth.c>
#include <curl/../../lib/version.c>
#include <curl/../../lib/version_win32.c>
#include <curl/../../lib/vquic/curl_msh3.c>
#include <curl/../../lib/vquic/curl_ngtcp2.c>
#include <curl/../../lib/vquic/curl_quiche.c>
#include <curl/../../lib/vquic/vquic.c>
#include <curl/../../lib/vssh/libssh.c>
#include <curl/../../lib/vssh/libssh2.c>
#include <curl/../../lib/vssh/wolfssh.c>
#include <curl/../../lib/vtls/bearssl.c>
#include <curl/../../lib/vtls/gtls.c>
#include <curl/../../lib/vtls/hostcheck.c>
#include <curl/../../lib/vtls/keylog.c>
#include <curl/../../lib/vtls/mbedtls.c>
#include <curl/../../lib/vtls/mbedtls_threadlock.c>
#include <curl/../../lib/vtls/openssl.c>
#include <curl/../../lib/vtls/rustls.c>
#include <curl/../../lib/vtls/schannel.c>
#include <curl/../../lib/vtls/schannel_verify.c>
#include <curl/../../lib/vtls/sectransp.c>
#include <curl/../../lib/vtls/vtls.c>
#include <curl/../../lib/vtls/wolfssl.c>
#include <curl/../../lib/vtls/x509asn1.c>
#include <curl/../../lib/warnless.c>
#include <curl/../../lib/ws.c>