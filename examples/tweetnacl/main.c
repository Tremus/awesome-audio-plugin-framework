#ifdef _WIN32
#define _CRT_RAND_S
#else
#include <fcntl.h>
#include <unistd.h>
#endif
#include "tweetnacl.h"
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

int main()
{
    unsigned char alice_pub[crypto_box_PUBLICKEYBYTES];
    unsigned char alice_sec[crypto_box_SECRETKEYBYTES];
    unsigned char bob_pub[crypto_box_PUBLICKEYBYTES];
    unsigned char bob_sec[crypto_box_SECRETKEYBYTES];
    const char*   msg   = "Alice's secret message!";
    size_t        msize = strlen(msg);

    // Create keypairs
    crypto_box_keypair(alice_pub, alice_sec);
    crypto_box_keypair(bob_pub, bob_sec);

    // Base 64 constants (Used for displaying as string)
    int public_encode_len = Base64encode_len(crypto_box_PUBLICKEYBYTES);
    int secret_encode_len = Base64encode_len(crypto_box_SECRETKEYBYTES);

    // Display Alice public key
    char alice_pub_enc[public_encode_len];
    Base64encode(alice_pub_enc, (const char*)alice_pub, crypto_box_PUBLICKEYBYTES);
    printf("Alice public key: %s\n", alice_pub_enc);

    // Display Alice secret key
    char alice_sec_enc[secret_encode_len];
    Base64encode(alice_sec_enc, (const char*)alice_sec, crypto_box_PUBLICKEYBYTES);
    printf("Alice secret key: %s\n", alice_sec_enc);

    // Display Bob public key
    char bob_pub_enc[public_encode_len];
    Base64encode(bob_pub_enc, (const char*)bob_pub, crypto_box_PUBLICKEYBYTES);
    printf("Bob public key: %s\n", bob_pub_enc);

    // Display Bob secret key
    char bob_sec_enc[secret_encode_len];
    Base64encode(bob_sec_enc, (const char*)bob_sec, crypto_box_PUBLICKEYBYTES);
    printf("Bob secret key: %s\n", bob_sec_enc);

    // Display message
    printf("Message: %s\n", msg);

    // Here we will pretend to be Alice, sending a message to Bob
    // She needs:
    // 1. A NONCE
    unsigned char anonce[crypto_box_NONCEBYTES];
    randombytes(anonce, sizeof(anonce));
    // 2. i/o buffers
    size_t         abufsize = msize + crypto_box_ZEROBYTES;
    unsigned char* ain      = malloc(abufsize);
    unsigned char* aout     = malloc(abufsize);
    // 3. Fill input buffer with Alice's message, padded with 32 zeros
    memset(ain, 0, crypto_box_ZEROBYTES);
    memcpy(ain + crypto_box_ZEROBYTES, msg, msize);

    // Encrypt alice encrypts message using Bobs public key
    int result = crypto_box(aout, ain, abufsize, anonce, bob_pub, alice_sec);
    // Display as base 64
    int  enc_len = Base64encode_len(abufsize);
    char aout_enc[enc_len];
    Base64encode(aout_enc, (const char*)aout, abufsize);
    printf("Result: %d\n", result);
    printf("Encrypted message: %s\n", aout_enc);

    // Presumably Alice's message will be saved to a file or sent over a
    // network. The message should consist of a NONCE part and encrypted message
    // part. Because the NONCE is a fixed size, she'll simply prefix her message
    // with the nonce.
    size_t         esize = abufsize - crypto_box_BOXZEROBYTES + crypto_box_NONCEBYTES;
    unsigned char* emsg  = malloc(esize);
    memcpy(emsg, anonce, crypto_box_NONCEBYTES); // prefix
    memcpy(emsg + crypto_box_NONCEBYTES, aout + crypto_box_BOXZEROBYTES, esize - crypto_box_NONCEBYTES);
    // This example will not save any files, but you would use emsg & esize
    // in your fopen/fwrite calls.

    // Now we will pretend to be Bob, who will decrypt Alice's message.
    // Bob only has knowledge of emsg, esize, and Alices public key.
    // He needs:
    // 1. The NONCE, which Alice prefixed in her message
    unsigned char bnonce[crypto_box_NONCEBYTES];
    memcpy(bnonce, emsg, crypto_box_NONCEBYTES);
    // 2. i/o buffers
    size_t         bbufsize = esize - crypto_box_NONCEBYTES + crypto_box_BOXZEROBYTES;
    unsigned char* bin      = malloc(bbufsize);
    unsigned char* bout     = malloc(bbufsize);
    // The enrypted message, formatted with 16 padded zeros
    memset(bin, 0, crypto_box_BOXZEROBYTES);
    memcpy(bin + crypto_box_BOXZEROBYTES, emsg + crypto_box_NONCEBYTES, esize - crypto_box_NONCEBYTES);

    result = crypto_box_open(bout, bin, bbufsize, bnonce, alice_pub, bob_sec);
    printf("Result: %d\n", result);
    // NULL terminate our string for printf
    bout[crypto_box_ZEROBYTES + msize - 1] = '\0';
    printf("Decrypted message: %s\n", bout + crypto_box_ZEROBYTES);

    // Hoorah!

    free(emsg);
    free(bout);
    free(bin);
    free(aout);
    free(ain);

    return 0;
}