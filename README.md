# Awesome Audio Plugin Framework

Curated list of small C/C++ libraries for building your own custom audio plugin framework.

Small libraries usually follow the [Unix philosophy](https://en.wikipedia.org/wiki/Unix_philosophy#Do_One_Thing_and_Do_It_Well), making them more maintainable, replaceable, and flexible in how they are used. Frameworks, by definition, break this philosophy.

This lists aims to support Windows, MacOS, and usually Linux. iOS, Android and Emscripten are not actively supported (open to any PRs!).

## Licensing

Dual & multi licensing schemes usually default to a kind of GPL and offer a conditional alternative. Sometimes it requiring a fee, sometimes it's due to a dependancy, which may be replaceable. Visit their repos and websites to learn about exact conditions.

## C

| Function           | Name                                                                 | License   | Description                                                                                                                                                    |
| ------------------ | -------------------------------------------------------------------- | --------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Window             | [sokol](https://github.com/floooh/sokol)                             | zlib      | Native window with mouse & keyboard callbacks. See: `sokol_app.h`.                                                                                             |
| VST3               | [vst3_c_api](https://github.com/steinbergmedia/vst3_c_api)           | Dual      | VST3 wrapper                                                                                                                                                   |
| CLAP               | [clap](https://github.com/free-audio/clap)                           | MIT       | CLAP wrapper                                                                                                                                                   |
| Threads            | [tinycthread](https://github.com/tinycthread/tinycthread)            | BSD like  | C11 threads.h like API, cross platform wrapper                                                                                                                 |
| File stream        | [sokol](https://github.com/floooh/sokol)                             | zlib      | Read & write stream for local files. See `sokol_fetch.h`                                                                                                       |
| Log                | [sokol](https://github.com/floooh/sokol)                             | zlib      | Log to console or file. See `sokol_log.h`                                                                                                                      |
| Log                | [stb](https://github.com/nothings/stb)                               | MIT       | Fast log to console. See `stb_sprintf.h`                                                                                                                       |
| Audio thread       | [sokol](https://github.com/floooh/sokol)                             | zlib      | Spins up an audio thread with buffer to write to. See `sokol_log.h`                                                                                            |
| HTTPS              | [curl](https://github.com/curl/curl)                                 | BSD like  | Makes HTTPS requests                                                                                                                                           |
| Time               | [sokol](https://github.com/floooh/sokol)                             | zlib      | Get & parse high resolution time. See `sokol_time.h`                                                                                                           |
| Image codecs       | [stb](https://github.com/nothings/stb)                               | MIT       | Load/decode JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC from file & memory. See `stb_image.h`, `stb_image_resize.h`, `stb_image_write.h`                            |
| JSON               | [zzzJSON](https://github.com/dacez/zzzjson)                          | BSD like  | Fast JSON encode/decode. Repo has benchmarks and links to alternatives                                                                                         |
| Crypto             | [TweetNaCl](https://tweetnacl.cr.yp.to/software.html)                | Unkown    | HMAC hashing, Public/Secret key encryption and signatures                                                                                                      |
| Vector graphics    | [NanoVG](https://github.com/memononen/nanovg)                        | zlib      | Antialised 2D polygons, lines, text, gradients and images. Backends for OpenGL, Metal, and DirectX 11. [Usefull repo](https://github.com/Tremus/nanovg_compat) |
| Regex              | [TRE](https://github.com/laurikari/tre)                              | BSD like  | Regex pattern matching. Great name                                                                                                                             |
| Regex              | [PCRE2](https://github.com/PCRE2Project/pcre2)                       | Dual      | Regex pattern matching                                                                                                                                         |
| Regex              | [tiny-regex-c](https://github.com/kokke/tiny-regex-c)                | Unlicense | Regex pattern matching                                                                                                                                         |
| File/folder window | [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) | zlib      | Native select file/folder dialogue boxes, alerts, colour picker & more                                                                                         |

## C++

| Function           | Name                                                                   | License  | Description                                           |
| ------------------ | ---------------------------------------------------------------------- | -------- | ----------------------------------------------------- |
| Window             | [CrossWindow](https://github.com/alaingalvan/CrossWindow)              | zlib     | Native window with mouse & keyboard callbacks.        |
| VST3               | [ vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)  | Dual     | VST3 wrapper                                          |
| AU                 | [ vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)  | Dual     | AU wrapper via a VST3 wrapper wrapper                 |
| AUv3               | [ vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)  | Dual     | AUv3 wrapper via a VST3 wrapper wrapper               |
| AAX                | [ vst3_public_sdk](https://github.com/steinbergmedia/vst3_public_sdk)  | Dual     | AAX wrapper via a VST3 wrapper wrapper                |
| VST3               | [clap-wrapper](https://github.com/free-audio/clap-wrapper)             | Multi    | VST3 wrapper via a CLAP wrapper                       |
| File read/write    | [cppfs](https://github.com/cginternals/cppfs)                          | MIT      | Manage local, virtual and remote files                |
| Audio codecs       | [CHOC](https://github.com/Tracktion/choc)                              | ISC      | Decode/encode WAV, FLAC, Ogg-Vorbis, decode only MP3. |
| MIDI parsing       | [CHOC](https://github.com/Tracktion/choc)                              | ISC      | Parse MIDI messages, MIDI sequences and MIDI files.   |
| MIDI/Audio sync    | [CHOC](https://github.com/Tracktion/choc)                              | ISC      | Splits an audio block to match times of MIDI events   |
| FFT                | [KFR](https://github.com/kfrlib/kfr)                                   | Dual     | Splits an audio block to match times of MIDI events   |
| Lock free queue    | [readerwriterqueue](https://github.com/cameron314/readerwriterqueue)   | BSD      | Splits an audio block to match times of MIDI events   |
| Timer thread       | [CHOC](https://github.com/Tracktion/choc)                              | ISC      | Timed callbacks                                       |
| Machine ID         | [machineid](https://github.com/Tarik02/machineid)                      | Apache 2 | Generates a unique string based on users hardware     |
| Base64             | [CHOC](https://github.com/Tracktion/choc)                              | ISC      | Base64 string encode/decode                           |
| Regex              | [hyperscan](https://github.com/intel/hyperscan)                        | BSD3     | Regex pattern matching                                |
| File/folder window | [tinyfiledialogs](https://github.com/samhocevar/portable-file-dialogs) | WTFPL    | Native select file/folder dialogue boxes              |
| JUCE               | [JUCE](https://github.com/juce-framework/JUCE)                         | ISC      | JUCE is an open-source cross-platform C++ application framework for desktop and mobile applications, including VST, VST3, AU, AUv3, RTAS and AAX audio plug-ins.              |

## TODO

Things to add to the list:

-   File searching
-   Graphics engine agnostic customisable widgets
-   Identify hardware, available CPU instructions at runtime
-   Something else? (suggest more!)
