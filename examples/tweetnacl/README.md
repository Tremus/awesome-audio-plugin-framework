# TweetNaCl

General notes on TweetNaCl:

Any string you need to encrypt will need to be put into an input buffer padded/prefixed with 32 zeros. The output buffer must be of equal length, and when encrypted it will be padded with 16 zeros.

Similarly when decrypting, the input & output buffer string must be of equal length with the input being padded with 16 zeros. When decrypted, the output buffer will have a padding of 32.

Encrypting will require a NONCE of 24 bytes. It's up to you to make sure a new random NONCE is used each time a message is encrypted, and that the random bytes are cryptographically strong.
