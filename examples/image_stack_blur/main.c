#define _CRT_SECURE_NO_WARNINGS
// This example only uses a jpg
#define STBI_ONLY_JPEG

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "stackblur.c"

void fix_filepath_windows(char* filepath)
{
#ifdef _WIN32
    // Simple fix for windows filepaths
    for (char* c = filepath; *c != '\0'; c++)
        if (*c == '/')
            *c = '\\';
#endif
}

int main()
{
    char filepath[256];

    // Load image
    // NOTE: We set the CMAKE_CURRENT_SOURCE_DIR preprocessor symbol in CMakeLists.txt
    snprintf(filepath, sizeof(filepath), "%s/brain.jpg", CMAKE_CURRENT_SOURCE_DIR);
    fix_filepath_windows(filepath);

    int       width, height, channels_in_file;
    const int desired_channels = 4;

    printf("Opening: %s\n", filepath);
    unsigned char* data = stbi_load(filepath, &width, &height, &channels_in_file, desired_channels);
    if (data == NULL)
    {
        printf("failed opening file\n");
        return 1;
    }

    printf("Image info - width: %d, height: %d, bit depth: %d\n", width, height, (channels_in_file * 8));

    // Do blur
    const int step           = 2;
    const int blur_radius_px = 20; // 2-254
    printf("Blurring image by %dpx\n", blur_radius_px);

    unsigned char stack[(blur_radius_px * 2 + 1) * 4];
    stackblurJob(data, width, height, blur_radius_px, 1, 0, step, stack);

    // Save image
    snprintf(filepath, sizeof(filepath), "%s/brain_blur.jpg", CMAKE_CURRENT_SOURCE_DIR);
    fix_filepath_windows(filepath);

    printf("Writing: %s\n", filepath);
    int quality = 80; // 1 - 100
    int success = stbi_write_jpg(filepath, width, height, desired_channels, data, quality);
    printf("Success: %d\n", success);

    stbi_image_free(data);

    return success == 1 ? 0 : 1;
}