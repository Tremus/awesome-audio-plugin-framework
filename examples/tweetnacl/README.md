# TweetNaCl

General notes on TweetNaCl:

You will require a input & output buffers of equal length as well as a NONCE.

The message you encrypt must be put within an input buffer padded/prefixed with 32 bytes, all zeros. Once encrypted, the output buffer will be padded with 16 zeros. When decrypting, you must maintain this 16 zero byte padding within your input.

Your NONCE must be 24 bytes in legnth. It's up to you to make sure a new random NONCE is used each time our message is encrypted, and that the random bytes are cryptographically strong.
