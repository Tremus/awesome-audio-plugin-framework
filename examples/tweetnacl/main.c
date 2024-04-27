#ifdef _WIN32
#define _CRT_RAND_S
#else
#include <fcntl.h>
#include <unistd.h>
#endif
#include "tweetnacl.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include an implementation to convert message to base 64 so
// encoded messages are shown as characters only.
#include "base64.h"

static int fd = -1;

void randombytes(unsigned char* x, unsigned long long xlen)
{
#ifdef _WIN32
    // My own implemetation based on 'rand_s', a Windows specific RNG. They say in their docs it's secure...
    // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/rand-s?view=msvc-170

    errno_t  err   = 0;
    unsigned bytes = 0;

    while (xlen != 0)
    {
        err = rand_s(&bytes);
        if (err != 0)
            printf_s("The rand_s function failed!\n");
        else
        {
            size_t num = sizeof(bytes) > xlen ? xlen : sizeof(bytes);

            memcpy(x, &bytes, num);

            x    += num;
            xlen -= num;
        }
    }
#else
    // This implementation of randombytes comes from the NaCl library:
    // https://nacl.cr.yp.to/install.html
    int i;

    if (fd == -1)
    {
        for (;;)
        {
            fd = open("/dev/urandom", O_RDONLY);
            if (fd != -1)
                break;
            sleep(1);
        }
    }

    while (xlen > 0)
    {
        if (xlen < 1048576)
            i = xlen;
        else
            i = 1048576;

        i = read(fd, x, i);
        if (i < 1)
        {
            sleep(1);
            continue;
        }

        x    += i;
        xlen -= i;
    }
#endif
}

void print_b64(const char* msg, unsigned char* data, size_t datalen)
{
    char encodebuf[256];
    assert(sizeof(encodebuf) > Base64encode_len(datalen));
    Base64encode(encodebuf, data, datalen);
    printf("%s: %s\n", msg, encodebuf);
}

int main()
{
    unsigned char alice_pub[crypto_box_PUBLICKEYBYTES];
    unsigned char alice_sec[crypto_box_SECRETKEYBYTES];
    unsigned char bob_pub[crypto_box_PUBLICKEYBYTES];
    unsigned char bob_sec[crypto_box_SECRETKEYBYTES];
    const char*   alice_msg    = "Alice's secret message!";
    size_t        alice_msglen = strlen(alice_msg);

    // Create keypairs
    crypto_box_keypair(alice_pub, alice_sec);
    crypto_box_keypair(bob_pub, bob_sec);

    // Base 64 constants (Used for displaying as string)
    int public_encode_len = Base64encode_len(crypto_box_PUBLICKEYBYTES);
    int secret_encode_len = Base64encode_len(crypto_box_SECRETKEYBYTES);

    // Display keys
    print_b64("Alice public key", alice_pub, sizeof(alice_pub));
    print_b64("Alice secret key", alice_sec, sizeof(alice_sec));
    print_b64("Bob public key", bob_pub, sizeof(bob_pub));
    print_b64("Bob secret key", bob_sec, sizeof(bob_sec));

    // Display message
    printf("Alice message: %s\n", alice_msg);

    // Here we will pretend to be Alice, sending a message to Bob
    // She needs:
    // 1. A NONCE
    unsigned char alice_nonce[crypto_box_NONCEBYTES];
    randombytes(alice_nonce, sizeof(alice_nonce));
    // 2. i/o buffers for encryption
    size_t         alice_io_len = alice_msglen + crypto_box_ZEROBYTES;
    unsigned char* alice_io_buf = malloc(alice_io_len * 2);
    unsigned char* alice_in     = alice_io_buf;
    unsigned char* alice_out    = alice_io_buf + alice_io_len;
    // 3. Fill input buffer with Alice's message, prefixed with 32 zeros
    memset(alice_in, 0, crypto_box_ZEROBYTES);
    memcpy(alice_in + crypto_box_ZEROBYTES, alice_msg, alice_msglen);

    // Alice encrypts message using Bobs public key
    // The result of this box function will prefix alice_out with 16 zeros
    int result = crypto_box(alice_out, alice_in, alice_io_len, alice_nonce, bob_pub, alice_sec);
    assert(result == 0);
    printf("Encrypt result: %d\n", result);

    // Presumably Alice's message will be saved to a file or sent over a
    // network. The message should consist of a NONCE part and encrypted message
    // part. Because the NONCE is a fixed size, she'll simply prefix her message
    // with the nonce.
    size_t         alice_encrypt_buflen = alice_io_len - crypto_box_BOXZEROBYTES + crypto_box_NONCEBYTES;
    unsigned char* alice_encrypt_msg    = malloc(alice_encrypt_buflen);
    memcpy(alice_encrypt_msg, alice_nonce, crypto_box_NONCEBYTES); // prefix
    memcpy(
        alice_encrypt_msg + crypto_box_NONCEBYTES,
        alice_out + crypto_box_BOXZEROBYTES,
        alice_encrypt_buflen - crypto_box_NONCEBYTES);

    // This example will not save any files, but we will pretend to
    size_t alice_encode_len = Base64encode_len(alice_encrypt_buflen);
    char*  alice_encode_buf = malloc(alice_encode_len);
    Base64encode(alice_encode_buf, (const char*)alice_encrypt_msg, alice_encrypt_buflen);
    printf("Alice encrypted & encoded message: %s\n", alice_encode_buf);

    // Now we will pretend to be Bob, who will decode & decrypt Alice's message.
    // Bob only has knowledge of alice_encode_buf, alice_encode_len, and Alices public key.
    // He needs:
    // 1. Decode the Base64 message

    // NOTE: this base64 lib will always add a trailing 0 to strings
    size_t bob_decode_len = Base64decode_len(alice_encode_buf) - 1;
    char*  bob_decode_buf = malloc(bob_decode_len + 1);
    Base64decode(bob_decode_buf, alice_encode_buf);
    assert(bob_decode_len == alice_encrypt_buflen);

    // 2. The NONCE, which Alice prefixed in her message
    unsigned char* bob_nonce = bob_decode_buf;
    // 3. i/o buffers
    size_t         bob_io_len = bob_decode_len - crypto_box_NONCEBYTES + crypto_box_BOXZEROBYTES;
    unsigned char* bob_io_buf = malloc(bob_io_len * 2);
    unsigned char* bob_in     = bob_io_buf;
    unsigned char* bob_out    = bob_io_buf + bob_io_len;
    // The enrypted message, formatted with 16 padded zeros
    memset(bob_in, 0, crypto_box_BOXZEROBYTES);
    memcpy(
        bob_in + crypto_box_BOXZEROBYTES,
        bob_decode_buf + crypto_box_NONCEBYTES,
        bob_decode_len - crypto_box_NONCEBYTES);

    result = crypto_box_open(bob_out, bob_in, bob_io_len, bob_nonce, alice_pub, bob_sec);
    assert(result == 0);
    printf("Decrypt result: %d\n", result);
    printf(
        "Bob decoded & decrypted message: %.*s\n",
        (int)(bob_io_len - crypto_box_ZEROBYTES),
        bob_out + crypto_box_ZEROBYTES);

    // Hoorah!

    free(alice_encode_buf);
    free(alice_encrypt_msg);
    free(alice_io_buf);
    free(bob_decode_buf);
    free(bob_io_buf);

    return 0;
}