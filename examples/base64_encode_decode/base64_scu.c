#define BASE64_STATIC_DEFINE

#include <../lib/lib.c>
#include <../lib/codec_choose.c>

#include <../lib/tables/tables.c>

// codec implementations
#include <../lib/arch/generic/codec.c>

#include <../lib/arch/ssse3/codec.c>
#include <../lib/arch/sse41/codec.c>
#include <../lib/arch/sse42/codec.c>
#include <../lib/arch/avx/codec.c>
#include <../lib/arch/avx2/codec.c>
#include <../lib/arch/avx512/codec.c>

#include <../lib/arch/neon32/codec.c>
#include <../lib/arch/neon64/codec.c>