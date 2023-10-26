// Cannot build pffft on Windows without math.h constants (eg. M_PI)
#define _USE_MATH_DEFINES
#include <pffft.h>
#include <stdio.h>

int main()
{
    const size_t N = (1 << 12);

    PFFFT_Setup* setup_real = pffft_new_setup(N, PFFFT_REAL);

    float in[N];
    float out[N];

    // fill input with saw wave, range -1.0f - 1.0f
    float acc = -1.0f;
    float inc = 2.0f / (float)N;
    for (int i = 0; i < N; i++)
    {
        in[i] = acc;
        acc   += inc;
    }

    pffft_transform_ordered(setup_real, in, out, NULL, PFFFT_FORWARD);

    // output is in rectangular form, not normalised
    // interleaved phase (-1 - 1) & magnitude
    for (int i = 0; i < N; i += 2)
        printf("phase: %f mag: %f\n", out[i], out[i + 1]);

    pffft_destroy_setup(setup_real);

    return 0;
}

#include <pffft.c>