# Awesome Audio Plugin Framework

Curated list of small C/C++ libraries for building your own custom audio plugin framework.

Small libraries usually follow the [Unix philosophy](https://en.wikipedia.org/wiki/Unix_philosophy#Do_One_Thing_and_Do_It_Well), making them more maintainable, replaceable, and flexible in how they are used. Frameworks, by definition, break this philosophy.

See the [Examples](./examples) folder for code & build exmaples of these libraries.

In future we hope to provide [plugin templates](./templates) based on small libraries.

## Values

The listed libraries and example code are generally biased towards:

-   Permissive non-GPL licensing
-   Support for Windows & MacOS. (Linux, iOS, Android and Emscripten are not actively supported!)
-   Ease of compilation, fast builds, small binaries, fewer LOCs, single compilation units, C89 & C99

## C

| Function           | Name                                                                 | License   | Description                                                                                                                                                    |
| ------------------ | -------------------------------------------------------------------- | --------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Window             | [Pugl](https://github.com/lv2/pugl)                                  | ISC       | Native window with mouse & keyboard callbacks. Made with plugins in mind                                                                                       |
| Window             | [sokol](https://github.com/floooh/sokol)                             | zlib      | Native window with mouse & keyboard callbacks. Made with games in mind. See: `sokol_app.h`.                                                                    |
| VST3               | [vst3_c_api](https://github.com/steinbergmedia/vst3_c_api)           | Dual      | VST3 wrapper                                                                                                                                                   |
| CLAP               | [clap](https://github.com/free-audio/clap)                           | MIT       | CLAP wrapper                                                                                                                                                   |
| Threads            | [threads.h](https://github.com/mattiasgustavsson/libs)               | Unlicense | C11 like threads & atomics, mutexes, singals, single reader/writer queues, cross platform                                                                      |
| Threads            | [tinycthread](https://github.com/tinycthread/tinycthread)            | BSD like  | C11 threads.h like API, cross platform                                                                                                                         |
| Atomics            | [c89atomic](https://github.com/mackron/c89atomic)                    | Unlicense | C11 like atomic.h like API, supports flags, float, double and ptr, cross platform                                                                              |
| File stream        | [sokol](https://github.com/floooh/sokol)                             | zlib      | Read & write stream for local files. See `sokol_fetch.h`                                                                                                       |
| Log                | [sokol](https://github.com/floooh/sokol)                             | zlib      | Log to console or file. See `sokol_log.h`                                                                                                                      |
| Log                | [stb](https://github.com/nothings/stb)                               | MIT       | Fast log to console. See `stb_sprintf.h`                                                                                                                       |
| Audio thread       | [sokol](https://github.com/floooh/sokol)                             | zlib      | Spins up an audio thread with buffer to write to. See `sokol_audio.h`                                                                                          |
| Audio IO           | [libsoundio](https://github.com/andrewrk/libsoundio)                 | MIT       | Wrapper for CoreAudio, WASAPI + more                                                                                                                           |
| HTTPS              | [curl](https://github.com/curl/curl)                                 | BSD like  | Makes HTTPS requests                                                                                                                                           |
| TLS                | [cute_headers](https://github.com/RandyGaul/cute_headers)            | Unlicense | HTTPS request. Lacks any parsing of responses. See `cute_tls.h`                                                                                                |
| HTTP parsing       | [picohttpparser](https://github.com/h2o/picohttpparser)              | MIT       | Parses HTTP1 requests & responses                                                                                                                              |
| Time               | [sokol](https://github.com/floooh/sokol)                             | zlib      | Get & parse high resolution time. See `sokol_time.h`                                                                                                           |
| Image codecs       | [stb](https://github.com/nothings/stb)                               | MIT       | Load/decode many image formats. See `stb_image.h`, `stb_image_resize.h`, `stb_image_write.h`                                                                   |
| Data containers    | [stb](https://github.com/nothings/stb)                               | MIT       | Variable type dynamic arrays and hash maps. See `stb_ds.h`                                                                                                     |
| Strings            | [sds](https://github.com/antirez/sds)                                | BSD-2     | Dyanamic size strings with manipulation and formatting methods.                                                                                                |
| JSON               | [zzzJSON](https://github.com/dacez/zzzjson)                          | BSD like  | Fast JSON encode/decode. Repo has benchmarks and links to alternatives                                                                                         |
| Crypto             | [TweetNaCl](https://tweetnacl.cr.yp.to/software.html)                | Unkown    | HMAC hashing, Public/Secret key encryption and signatures                                                                                                      |
| Vector graphics    | [NanoVG](https://github.com/memononen/nanovg)                        | zlib      | Antialised 2D polygons, lines, text, gradients and images. Backends for OpenGL, Metal, and DirectX 11. [Usefull repo](https://github.com/Tremus/nanovg_compat) |
| Regex              | [TRE](https://github.com/laurikari/tre)                              | BSD like  | Regex pattern matching. Great name                                                                                                                             |
| Regex              | [PCRE2](https://github.com/PCRE2Project/pcre2)                       | Dual      | Regex pattern matching                                                                                                                                         |
| Regex              | [tiny-regex-c](https://github.com/kokke/tiny-regex-c)                | Unlicense | Regex pattern matching                                                                                                                                         |
| File/folder window | [osdialog](https://github.com/AndrewBelt/osdialog)                   | CC0       | Native select file/folder dialogue boxes, alerts, colour picker & more                                                                                         |
| File/folder window | [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) | zlib      | Native select file/folder dialogue boxes, alerts, colour picker & more                                                                                         |
| Binary embed       | [bin2c](https://github.com/adobe/bin2c)                              | Apache 2  | CLI. Turns single file (eg. font, image) into `.c` containg `const char[]`.                                                                                    |
| Binary embed       | [c-embed](https://github.com/weigert/c-embed)                        | MIT       | CLI. Turns multiple files (eg. font, image) into single `.o`. Access in code using virtual file system                                                         |
| Base64             | [base64](https://github.com/aklomp/base64)                           | BSD-2     | Base64 string encode & decode. See [this](https://github.com/Tremus/base64/blob/master/include/libbase64_scu.c) for a simpler & faster build                   |

## C++

| Function           | Name                                                                         | License  | Description                                           |
| ------------------ | ---------------------------------------------------------------------------- | -------- | ----------------------------------------------------- |
| Window             | [CrossWindow](https://github.com/alaingalvan/CrossWindow)                    | zlib     | Native window with mouse & keyboard callbacks.        |
| VST3               | [vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)         | Dual     | VST3 wrapper                                          |
| AU                 | [vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)         | Dual     | AU wrapper via a VST3 wrapper wrapper                 |
| AUv3               | [vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)         | Dual     | AUv3 wrapper via a VST3 wrapper wrapper               |
| AAX                | [vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)         | Dual     | AAX wrapper via a VST3 wrapper wrapper                |
| VST3               | [clap-wrapper](https://github.com/free-audio/clap-wrapper)                   | Multi    | VST3 wrapper via a CLAP wrapper                       |
| File read/write    | [cppfs](https://github.com/cginternals/cppfs)                                | MIT      | Manage local, virtual and remote files                |
| Audio codecs       | [CHOC](https://github.com/Tracktion/choc)                                    | ISC      | Decode/encode WAV, FLAC, Ogg-Vorbis, decode only MP3. |
| MIDI parsing       | [CHOC](https://github.com/Tracktion/choc)                                    | ISC      | Parse MIDI messages, MIDI sequences and MIDI files.   |
| MIDI/Audio sync    | [CHOC](https://github.com/Tracktion/choc)                                    | ISC      | Splits an audio block to match times of MIDI events   |
| Log                | [fast_io](https://github.com/cppfastio/fast_io)                              | MIT      | \<iostream\> replacement with threadsafe options      |
| FFT                | [KFR](https://github.com/kfrlib/kfr)                                         | Dual     | Really fast FFTs.                                     |
| Lock free queue    | [readerwriterqueue](https://github.com/cameron314/readerwriterqueue)         | BSD      | Does queue. No lock                                   |
| Timer thread       | [CHOC](https://github.com/Tracktion/choc)                                    | ISC      | Timed callbacks                                       |
| Machine ID         | [machineid](https://github.com/Tarik02/machineid)                            | Apache 2 | Generates a unique string based on users hardware     |
| Base64             | [CHOC](https://github.com/Tracktion/choc)                                    | ISC      | Base64 string encode/decode                           |
| Regex              | [hyperscan](https://github.com/intel/hyperscan)                              | BSD3     | Regex pattern matching                                |
| File/folder window | [Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs) | WTFPL    | Native select file/folder dialogue boxes              |

## TODO

Things to add to the list:

-   File searching
-   Graphics engine agnostic customisable widgets
-   Something else? (suggest more!)
