#define CUTE_FILES_IMPLEMENTATION
#include "cute_files.h"
#include <stdio.h>

void file_cb(cf_file_t* file, void* udata)
{
    if (file->is_dir)
    {
        // MacOS will return /.. & /. entries
        if (0 == strcmp(file->name, ".") || 0 == strcmp(file->name, ".."))
            return;

        fprintf(stderr, "Searching dir: %s\n", file->path);
        // Handle valid directory
        // NOTE: You may want to skip empty directories, or directories containing empty directories etc.
        //       We don't check for that here, you'll have to figure that out yourself
    }
    if (file->is_reg)
    {
        const char* ext = cf_get_ext(file);
        if (0 != strcmp(ext, ".cmake"))
            return;
        fprintf(stderr, "Found:         ./%s\n", file->name);
        // Handle valid file
    }
}

int main()
{
    // Recursively search through folders looking for '.cmake' files
    cf_traverse(BUILD_DIR, file_cb, NULL);
    return 0;
}