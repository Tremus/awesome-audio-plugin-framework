# Awesome Audio Plugin Framework

Curated list of small C/C++ libraries for building your own custom audio plugin framework.

Small libraries usually follow the [Unix philosophy](https://en.wikipedia.org/wiki/Unix_philosophy#Do_One_Thing_and_Do_It_Well), making them more maintainable, replaceable, and flexible in how they are used. Frameworks, by definition, break this philosophy.

See the [Examples](./examples) folder for code & build examples of these libraries.

See the [Templates](./templates) folder for ready to go, lightweight plugin templates.

The listed libraries and example code are generally biased towards:

-   Permissive non-GPL licensing
-   Support for Windows & MacOS. (Linux, iOS, Android and Emscripten are not actively supported!)
-   Ease of compilation, fast builds, small binaries, fewer LOCs, single compilation units, C89 & C99

## C

| Function        | Name                                                                 | License   | Description                                                                                     |
| --------------- | -------------------------------------------------------------------- | --------- | ----------------------------------------------------------------------------------------------- |
| Plugin wrapper  | [CPLUG](https://github.com/Tremus/CPLUG)                             | Unlicense | Wrapper for VST3, AUv2, CLAP, standalone. Includes optional window lib for plugins              |
| Window          | [Pugl](https://github.com/lv2/pugl)                                  | ISC       | Native window with mouse & keyboard callbacks. Made with plugins in mind                        |
| Window          | [sokol](https://github.com/floooh/sokol)                             | zlib      | Native window with mouse & keyboard callbacks. Made with games in mind. See: `sokol_app.h`.     |
| VST3            | [vst3_c_api](https://github.com/steinbergmedia/vst3_c_api)           | Dual      | VST3 wrapper                                                                                    |
| CLAP            | [clap](https://github.com/free-audio/clap)                           | MIT       | CLAP wrapper                                                                                    |
| Threads         | [threads.h](https://github.com/mattiasgustavsson/libs)               | Unlicense | C11 like threads & atomics, mutexes, singals, single reader/writer queues, cross platform       |
| Threads         | [tinycthread](https://github.com/tinycthread/tinycthread)            | BSD like  | C11 threads.h like API, cross platform                                                          |
| Atomics         | [c89atomic](https://github.com/mackron/c89atomic)                    | Unlicense | C11 like atomic.h like API, supports flags, float, double and ptr, cross platform               |
| WAV             | [dr_libs](https://github.com/mackron/dr_libs)                        | Unlicense | WAV read & write. See `dr_wav.h`                                                                |
| AIFF            | [dr_libs](https://github.com/mackron/dr_libs)                        | Unlicense | AIFF read & write. See `dr_wav.h`                                                               |
| MP3             | [dr_libs](https://github.com/mackron/dr_libs)                        | Unlicense | MP3 read. See `dr_mp3.h`                                                                        |
| FLAC            | [dr_libs](https://github.com/mackron/dr_libs)                        | Unlicense | FLAC read. See `dr_flac.h`                                                                      |
| FLAC            | [FLAC](https://github.com/xiph/flac)                                 | BSD-3     | FLAC encode / decode / read / write                                                             |
| Opus            | [Opus](https://github.com/xiph/opus)                                 | BSD-3     | Opus encode / decode / read / write                                                             |
| Log             | [sokol](https://github.com/floooh/sokol)                             | zlib      | Log to console or file. See `sokol_log.h`                                                       |
| Log             | [stb](https://github.com/nothings/stb)                               | MIT       | Fast log to console. See `stb_sprintf.h`                                                        |
| FFT             | [pffft](https://bitbucket.org/jpommier/pffft/src/master/)            | BSD like  | 1D real & complex FFTs. Competitively fast. SSE & NEON optimised                                |
| Audio thread    | [sokol](https://github.com/floooh/sokol)                             | zlib      | Spins up an audio thread with buffer to write to. See `sokol_audio.h`                           |
| Audio IO        | [libsoundio](https://github.com/andrewrk/libsoundio)                 | MIT       | Wrapper for CoreAudio, WASAPI + more                                                            |
| Vector graphics | [NanoVG](https://github.com/memononen/nanovg)                        | zlib      | Antialised 2D polygons, lines, text, gradients and images using OpenGL                          |
| Vector graphics | [nanovg_compat](https://github.com/Tremus/nanovg_compat)             | zlib      | NanoVG wrapper with Metal & DirectX 11 backends and extra functions for initialising a context. |
| Text Rendering  | [Freetype2](https://gitlab.freedesktop.org/freetype/freetype)        | FTL       | CPU rendering for most fonts for most devices.                                                  |
| Text Shaping    | [kb](https://github.com/JimmyLefevre/kb)                             | zlib      | Multilingual text segmentation and shaping                                                      |
| Memory          | [xhl](https://github.com/Tremus/xhl)                                 | Unlicense | Allocators with leak detectors. See `alloc.h`                                                   |
| Maths           | [xhl](https://github.com/Tremus/xhl)                                 | Dual      | Fast maths approximations. See `maths.h`                                                        |
| Time            | [xhl](https://github.com/Tremus/xhl)                                 | Unlicense | High res time, get OS time as unix timestamp, performance profiling tools. See `time.h`         |
| Time            | [sokol](https://github.com/floooh/sokol)                             | zlib      | Get & parse high resolution time. See `sokol_time.h`                                            |
| Image codecs    | [stb](https://github.com/nothings/stb)                               | MIT       | Load/decode many image formats. See `stb_image.h`, `stb_image_resize.h`, `stb_image_write.h`    |
| Data containers | [stb](https://github.com/nothings/stb)                               | MIT       | Variable type dynamic arrays and hash maps. See `stb_ds.h`                                      |
| Strings         | [sds](https://github.com/antirez/sds)                                | BSD-2     | Dyanamic size strings with manipulation and formatting methods.                                 |
| UTF8            | [utf8](https://github.com/sheredom/utf8.h)                           | Unlicense | stdlib string.h, locale.h, ctype.h replacement functions for `\0` terminated UTF8 strings       |
| UTF8            | [cute_headers](https://github.com/RandyGaul/cute_headers_deprecated) | Unlicense | UTF8 & UTF16 conversion functions. See `cute_utf8.h`                                            |
| Crypto          | [TweetNaCl](https://tweetnacl.cr.yp.to/software.html)                | PD        | HMAC hashing, Public/Secret key encryption and signatures                                       |
| Binary embed    | [bin2c](https://github.com/adobe/bin2c)                              | Apache 2  | CLI. Turns single file (eg. font, image) into `.c` containg `const char[]`.                     |
| File dialog box | [osdialog](https://github.com/AndrewBelt/osdialog)                   | CC0       | Native select file/folder dialogue boxes, alerts, colour picker & more                          |
| File dialog box | [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) | zlib      | Native select file/folder dialogue boxes, alerts, colour picker & more                          |
| File I/O        | [xhl](https://github.com/Tremus/xhl)                                 | Unlicense | File r/w/a, paths eg. Desktop/Documents, list dir & watch file change events. UTF8. `files.h`   |
| File searching  | [cute_headers](https://github.com/RandyGaul/cute_headers_deprecated) | Unlicense | Recursive file & directory search. See `cute_files.h`                                           |
| File stream     | [sokol](https://github.com/floooh/sokol)                             | zlib      | Read & write stream for local files. See `sokol_fetch.h`                                        |
| TLS             | [cute_headers](https://github.com/RandyGaul/cute_headers)            | Unlicense | HTTPS request. Lacks any parsing of responses. See `cute_tls.h`                                 |
| HTTPS           | [curl](https://github.com/curl/curl)                                 | BSD like  | Makes HTTPS requests                                                                            |
| HTTP parsing    | [picohttpparser](https://github.com/h2o/picohttpparser)              | MIT       | Parses HTTP1 requests & responses                                                               |
| JSON            | [yyJSON](https://github.com/ibireme/yyjson)                          | MIT       | 2nd fastest JSON encode/decode. Parses all data upfront to create a searchable linked list.     |
| JSON            | [zzzJSON](https://github.com/dacez/zzzjson)                          | BSD like  | Fast JSON encode/decode. Uses on the fly parsing and string searching and string views.         |
| Base64          | [base64](https://github.com/aklomp/base64)                           | BSD-2     | Base64 string encode & decode                                                                   |
| Regex           | [tiny-regex-c](https://github.com/kokke/tiny-regex-c)                | Unlicense | Regex pattern matching                                                                          |
| Regex           | [PCRE2](https://github.com/PCRE2Project/pcre2)                       | Dual      | Regex pattern matching                                                                          |
| Regex           | [TRE](https://github.com/laurikari/tre)                              | BSD like  | Regex pattern matching but with a greater name                                                  |

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
| JSON               | [simdjson](https://github.com/simdjson/simdjson)                             | Apache 2 | Fastest JSON encode/decode                            |
| Regex              | [hyperscan](https://github.com/intel/hyperscan)                              | BSD3     | Regex pattern matching                                |
| File/folder window | [Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs) | WTFPL    | Native select file/folder dialogue boxes              |

## Building the exmaples

All examples are built using CMake

Dependancies are are stored as git submodules.

Several of these dependencies use their own CMake build config, and we ignore all of them, often because they're too slow and complicated. All necessary build code is written in this repo.

```
git clone https://github.com/Tremus/awesome-audio-plugin-framework
cd awesome-audio-plugin-framework
git submodule update --init --depth=1 --recursive

cmake -B build -S .
cd build
cmake --build .
```

## TODO

Things to add to the list:

-   Graphics engine agnostic customisable widgets
-   Something else? (suggest more!)
