#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    char        buf[1024];
    const char* inpath   = NULL;
    const char* filename = NULL;
    const char* ext      = NULL;
    FILE*       in       = NULL;
    FILE*       out      = NULL;
    unsigned    bufsize  = 0;
    unsigned    filesize = 0;

    if (argc != 2)
        goto usage;

    // Find filename & file extension from path
    ext = filename = inpath = argv[1];

    for (const char* c = filename; *c != '\0'; c++)
        if (*c == '/')
            filename = c + 1;
    for (const char* c = filename; *c != '\0'; c++)
        if (*c == '.')
            ext = c;
    // Does this look like a filepath?
    if (inpath == filename)
        goto usage;

    // Write boilerplate header
    snprintf(buf, sizeof(buf), "%.*s_bin.h", (int)(ext - inpath), inpath);
    out = fopen(buf, "wb+");
    assert(out);
    if (out)
    {
        fprintf(out, "extern const unsigned char %.*s_buf[];\n", (int)(ext - filename), filename);
        fprintf(out, "extern const unsigned %.*s_len;", (int)(ext - filename), filename);
        fclose(out);
    }
    out = NULL;

    // Read bytes from input file.ext and write into file_bin.c
    in = fopen(inpath, "rb");

    snprintf(buf, sizeof(buf), "%.*s_bin.c", (int)(ext - inpath), inpath);
    out = fopen(buf, "w+");

    assert(in && out);
    if (in && out)
    {
        fprintf(out, "const unsigned char %.*s_buf[] = {\n", (int)(ext - filename), filename);
        while ((bufsize = fread(buf, 1, sizeof(buf), in)))
        {
            const int COL_WIDTH = 24;
            int       i         = 0;
            for (int row = 0; row < bufsize / COL_WIDTH; row++)
            {
                for (i = row * COL_WIDTH; i < (row + 1) * COL_WIDTH; i++)
                    fprintf(out, "%hhu, ", buf[i]);
                fprintf(out, "\n");
            }
            for (; i < bufsize; i++)
                fprintf(out, "%hhu, ", buf[i]);

            filesize += bufsize;
        }
        fprintf(out, "\n};\nconst unsigned %.*s_len = %u;", (int)(ext - filename), filename, filesize);
    }

    if (in)
        fclose(in);
    if (out)
        fclose(out);
    return 0;

usage:
    fprintf(stderr, "Cmd:");
    for (int i = 0; i < argc; i++)
        fprintf(stderr, "%s ", argv[i]);

    fprintf(stderr, "\nUsage: %s /path/to/file\n", argv[0]);
    return 1;
}