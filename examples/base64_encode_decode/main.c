// We have to tell the Base64 SCU which instruction sets we want to build with
#ifdef __x86_64__
    // +8kb release binary
    #define BASE64_HAVE_SSSE3 1
    // +7kb release binary
    #define BASE64_HAVE_AVX 1
    // +7kb release binary
    #define BASE64_HAVE_AVX2 1
    // +2kb release binary
    #define BASE64_HAVE_AVX512 1

    // SSE4.1 & 4.2 functions just call SSSE3 functions...
    // Building with these macros set to 1 just bloats the release binary by 15kb...
    #define BASE64_HAVE_SSE41 0
    #define BASE64_HAVE_SSE42 0
#elif __aarch64__
    #define BASE64_HAVE_NEON64 1
#elif __arm__ 
    // Don't bother supporting 32bit
    #define BASE64_HAVE_NEON32 0
#endif

#include <libbase64_scu.c>

int main ()
{
	size_t enc_len;
	char enc_buf[64];
	size_t dec_len;
	char dec_buf[64];

	char message[] = "hello world";
	size_t message_len = sizeof(message) - 1;

    printf("Orignal message (%d): %s\n", (int)message_len, message);

    printf("Encoding...\n");
	base64_encode(message, message_len, enc_buf, &enc_len, 0);
    enc_buf[enc_len] = '\0';

    printf("Encoded (%d): %s\n", (int)enc_len, enc_buf);

    printf("Decoding...\n");
	base64_decode(enc_buf, enc_len, dec_buf, &dec_len, 0);
    dec_buf[dec_len] = '\0';

    printf("Decoded (%d): %s\n", (int)dec_len, dec_buf);

    if (g_base64_codec.enc == base64_stream_encode_ssse3)
        printf("Ran using SSSE3!\n");
    if (g_base64_codec.enc == base64_stream_encode_avx)
        printf("Ran using AVX!\n");
    if (g_base64_codec.enc == base64_stream_encode_avx2)
        printf("Ran using AVX2!\n");
    if (g_base64_codec.enc == base64_stream_encode_avx512)
        printf("Ran using AVX512!\n");
    if (g_base64_codec.enc == base64_stream_encode_neon64)
        printf("Ran using NEON!\n");
    if (g_base64_codec.enc == base64_stream_encode_plain)
        printf("Ran without handwritten assembly...\n");

	return 0;
}