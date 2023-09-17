#include "tweetnacl.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int fd = -1;

// TweetNaCl makes you provide your own randomBytes implementation.
// NaCl provide one in their source, but it isn't Windows friendly...
// https://nacl.cr.yp.to/install.html
// TODO: find a cryptographically secure RNG for Windows...
void randombytes(unsigned char* x, unsigned long long xlen)
{
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

        x += i;
        xlen -= i;
    }
}

int main()
{
    unsigned char alice_pub[crypto_box_PUBLICKEYBYTES];
    unsigned char alice_sec[crypto_box_SECRETKEYBYTES];
    unsigned char bob_pub[crypto_box_PUBLICKEYBYTES];
    unsigned char bob_sec[crypto_box_SECRETKEYBYTES];
    const char* msg = "Alice's secret message!";
    size_t msize    = strlen(msg);

    crypto_box_keypair(alice_pub, alice_sec);
    crypto_box_keypair(bob_pub, bob_sec);

    printf("Alice public key: %s\n", alice_pub);
    printf("Alice secret key: %s\n", alice_sec);
    printf("Bob public key: %s\n", bob_pub);
    printf("Bob secret key: %s\n", bob_sec);
    printf("Message: %s\n", msg);

    // Here we will pretend to be Alice, sending a message to Bob
    // She needs:
    // 1. A NONCE
    unsigned char nonce[crypto_box_NONCEBYTES];
    randombytes(nonce, sizeof(nonce));
    // 2. i/o buffers
    size_t bufsize     = msize + crypto_box_ZEROBYTES;
    unsigned char* in  = malloc(bufsize);
    unsigned char* out = malloc(bufsize);
    // 3. Fill input buffer with Alice's message, padded with 32 zeros
    memset(in, 0, crypto_box_ZEROBYTES);
    memcpy(in + crypto_box_ZEROBYTES, msg, msize);

    // Encrypt alice encrypts message using Bobs public key
    int result = crypto_box(out, in, bufsize, nonce, bob_pub, alice_sec);
    printf("Result: %d\n", result);
    printf("Encrypted message: %s\n", out + crypto_box_BOXZEROBYTES);

    // Presumably Alice's message will be saved to a file or sent over a
    // network. The message should consist of a NONCE part and encrypted message
    // part. Because the NONCE is a fixed size, she'll simply prefix her message
    // with the nonce.
    size_t esize = bufsize - crypto_box_BOXZEROBYTES + crypto_box_NONCEBYTES;
    unsigned char* emsg = malloc(esize);
    memcpy(emsg, nonce, crypto_box_NONCEBYTES); // prefix
    memcpy(
        emsg + crypto_box_NONCEBYTES,
        out + crypto_box_BOXZEROBYTES,
        esize - crypto_box_NONCEBYTES);
    // This example will not save any files, but you would use emsg & esize
    // in your fopen/fwrite calls.

    free(out);
    free(in);
    in      = 0;
    out     = 0;
    bufsize = 0;
    memset(nonce, 0, sizeof(nonce));

    // Now we will pretend to be Bob, who will decrypt Alice's message.
    // Bob only has knowledge of emsg, esize, and Alices public key.
    // He needs:
    // 1. The NONCE, which is Alice prefixed in her message
    memcpy(nonce, emsg, crypto_box_NONCEBYTES);
    // 2. i/o buffers
    bufsize = esize - crypto_box_NONCEBYTES + crypto_box_BOXZEROBYTES;
    in      = malloc(bufsize);
    out     = malloc(bufsize);
    // The enrypted message, formatted with 16 padded zeros
    memset(in, 0, crypto_box_BOXZEROBYTES);
    memcpy(
        in + crypto_box_BOXZEROBYTES,
        emsg + crypto_box_NONCEBYTES,
        esize - crypto_box_NONCEBYTES);

    result = crypto_box_open(out, in, bufsize, nonce, alice_pub, bob_sec);
    printf("Result: %d\n", result);
    printf("Decrypted message: %s\n", out + crypto_box_ZEROBYTES);

    // Hoorah!

    free(emsg);
    free(out);
    free(in);

    return 0;
}