// Must include stdint.h before tinyfiledialogs.h
#include <stdint.h>
#include <stdio.h>

#include <tinyfiledialogs.h>

#define NUM_ELEMENTS(arr) (sizeof(arr) / sizeof((arr)[0]))

int main()
{
    char const* lTheOpenFileName;
    char const* lFilterPatterns[] =
        {"*.jpg", "*.jpeg", "*.png", "*.gif", "*.webp"};

    lTheOpenFileName = tinyfd_openFileDialog(
        "Show me a spicy meme",
        "",
        NUM_ELEMENTS(lFilterPatterns),
        lFilterPatterns,
        "Images",
        0);

    if (! lTheOpenFileName)
    {
        printf("Failed!\n");
        return 1;
    }
    else
    {
        printf("Spicy meme located at: %s\n", lTheOpenFileName);
    }

    return 0;
}