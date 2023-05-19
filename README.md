# Awesome Audio Plugin Framework

Curated list of small C/C++ libraries for building your own custom audio plugin framework.

Small libraries usually follow the [Unix philosophy](https://en.wikipedia.org/wiki/Unix_philosophy#Do_One_Thing_and_Do_It_Well), making them more maintainable, replaceable, and flexible in how they are used. Frameworks, by definition, break this philosophy.

This lists aims to support Windows, MacOS, and usually Linux. iOS, Android and Emscripten are not actively supported (open to any PRs!).

## Licensing

GPL and similar licenses that require disclosure of source are colour coded in <span style="color:orange;">orange</span>. Dual licensing schemes usually default to GPL but offer a conditional alternative, sometimes requiring a fee. Visit their repos and websites for more info.

BSD style licenses are coded in <span style="color:lime;">green</span>

## C

| Function           | Name                                                                 | License                                    | Description                                                                                                                                                    |
| ------------------ | -------------------------------------------------------------------- | ------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Window             | [sokol](https://github.com/floooh/sokol)                             | <span style="color:lime;">zlib</span>      | Native window with mouse & keyboard callbacks. See: `sokol_app.h`.                                                                                             |
| VST3               | [vst3_c_api](https://github.com/steinbergmedia/vst3_c_api)           | <span style="color:orange;">Dual</span>    | VST3 wrapper                                                                                                                                                   |
| CLAP               | [clap](https://github.com/free-audio/clap)                           | <span style="color:lime;">MIT</span>       | CLAP wrapper                                                                                                                                                   |
| Threads            | [tinycthread](https://github.com/tinycthread/tinycthread)            | <span style="color:lime;">BSD like</span>  | C11 threads.h like API, cross platform wrapper                                                                                                                 |
| File stream        | [sokol](https://github.com/floooh/sokol)                             | <span style="color:lime;">zlib</span>      | Read & write stream for local files. See `sokol_fetch.h`                                                                                                       |
| Log                | [sokol](https://github.com/floooh/sokol)                             | <span style="color:lime;">zlib</span>      | Log to console or file. See `sokol_log.h`                                                                                                                      |
| Log                | [stb](https://github.com/nothings/stb)                               | <span style="color:lime;">MIT</span>       | Fast log to console. See `stb_sprintf.h`                                                                                                                       |
| Audio thread       | [sokol](https://github.com/floooh/sokol)                             | <span style="color:lime;">zlib</span>      | Spins up an audio thread with buffer to write to. See `sokol_log.h`                                                                                            |
| HTTPS              | [curl](https://github.com/curl/curl)                                 | <span style="color:lime;">BSD like</span>  | Makes HTTPS requests                                                                                                                                           |
| Time               | [sokol](https://github.com/floooh/sokol)                             | <span style="color:lime;">zlib</span>      | Get & parse high resolution time. See `sokol_time.h`                                                                                                           |
| Image codecs       | [stb](https://github.com/nothings/stb)                               | <span style="color:lime;">MIT</span>       | Load/decode JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC from file & memory. See `stb_image.h`, `stb_image_resize.h`, `stb_image_write.h`                            |
| JSON               | [zzzJSON](https://github.com/dacez/zzzjson)                          | <span style="color:lime;">BSD like</span>  | Fast JSON encode/decode. Repo has benchmarks and links to alternatives                                                                                         |
| Crypto             | [TweetNaCl](https://tweetnacl.cr.yp.to/software.html)                | Unkown                                     | HMAC hashing, Public/Secret key encryption and signatures                                                                                                      |
| Vector graphics    | [NanoVG](https://github.com/memononen/nanovg)                        | <span style="color:lime;">zlib</span>      | Antialised 2D polygons, lines, text, gradients and images. Backends for OpenGL, Metal, and DirectX 11. [Usefull repo](https://github.com/Tremus/nanovg_compat) |
| Regex              | [TRE](https://github.com/laurikari/tre)                              | <span style="color:lime;">BSD like</span>  | Regex pattern matching. Great name                                                                                                                             |
| Regex              | [PCRE2](https://github.com/PCRE2Project/pcre2)                       | <span style="color:orange;">Dual</span>    | Regex pattern matching                                                                                                                                         |
| Regex              | [tiny-regex-c](https://github.com/kokke/tiny-regex-c)                | <span style="color:lime;">Unlicense</span> | Regex pattern matching                                                                                                                                         |
| File/folder window | [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) | <span style="color:lime;">zlib</span>      | Native select file/folder dialogue boxes, alerts, colour picker & more                                                                                         |

## C++

| Function           | Name                                                                   | License                                   | Description                                           |
| ------------------ | ---------------------------------------------------------------------- | ----------------------------------------- | ----------------------------------------------------- |
| Window             | [CrossWindow](https://github.com/alaingalvan/CrossWindow)              | <span style="color:lime;">zlib</span>     | Native window with mouse & keyboard callbacks.        |
| VST3               | [ vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)  | <span style="color:orange;">Dual</span>   | VST3 wrapper                                          |
| AU                 | [ vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)  | <span style="color:orange;">Dual</span>   | AU wrapper via a VST3 wrapper wrapper                 |
| AUv3               | [ vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)  | <span style="color:orange;">Dual</span>   | AUv3 wrapper via a VST3 wrapper wrapper               |
| AAX                | [ vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)  | <span style="color:orange;">Dual</span>   | AAX wrapper via a VST3 wrapper wrapper                |
| VST3               | [clap-wrapper](https://github.com/free-audio/clap-wrapper)             | <span style="color:orange;">Multi</span>  | VST3 wrapper via a CLAP wrapper                       |
| File read/write    | [cppfs](https://github.com/cginternals/cppfs)                          | <span style="color:lime;">MIT</span>      | Manage local, virtual and remote files                |
| Audio codecs       | [CHOC](https://github.com/Tracktion/choc)                              | <span style="color:lime;">ISC</span>      | Decode/encode WAV, FLAC, Ogg-Vorbis, decode only MP3. |
| MIDI parsing       | [CHOC](https://github.com/Tracktion/choc)                              | <span style="color:lime;">ISC</span>      | Parse MIDI messages, MIDI sequences and MIDI files.   |
| MIDI/Audio sync    | [CHOC](https://github.com/Tracktion/choc)                              | <span style="color:lime;">ISC</span>      | Splits an audio block to match times of MIDI events   |
| FFT                | [KFR](https://github.com/kfrlib/kfr)                                   | <span style="color:orange;">Dual</span>   | Splits an audio block to match times of MIDI events   |
| Lock free queue    | [readerwriterqueue](https://github.com/cameron314/readerwriterqueue)   | <span style="color:lime;">BSD</span>      | Splits an audio block to match times of MIDI events   |
| Timer thread       | [CHOC](https://github.com/Tracktion/choc)                              | <span style="color:lime;">ISC</span>      | Timed callbacks                                       |
| Machine ID         | [machineid](https://github.com/Tarik02/machineid)                      | <span style="color:lime;">Apache 2</span> | Generates a unique string based on users hardware     |
| Base64             | [CHOC](https://github.com/Tracktion/choc)                              | <span style="color:lime;">ISC</span>      | Base64 string encode/decode                           |
| Regex              | [hyperscan](https://github.com/intel/hyperscan)                        | <span style="color:lime;">BSD3</span>     | Regex pattern matching                                |
| File/folder window | [tinyfiledialogs](https://github.com/samhocevar/portable-file-dialogs) | <span style="color:lime;">WTFPL</span>    | Native select file/folder dialogue boxes              |

## TODO

Things to add to the list:

-   File searching
-   Graphics engine agnostic customisable widgets
-   Identify hardware, available CPU instructions at runtime
-   Something else? (suggest more!)
