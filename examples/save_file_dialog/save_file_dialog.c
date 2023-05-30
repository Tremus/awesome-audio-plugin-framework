// Must include stdint.h before tinyfiledialogs.h
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <tinyfiledialogs.h>

int main()
{
    char const* lTheSaveFileName;
    char const* lFilterPatterns[1] = {"*.txt"};
    FILE* lIn;

    lTheSaveFileName =
        tinyfd_saveFileDialog("Save File", NULL, 1, lFilterPatterns, NULL);

    if (! lTheSaveFileName)
    {
        printf("Failed!\n");
        return 1;
    }

#ifdef _WIN32
    if (tinyfd_winUtf8)
        lIn = _wfopen(
            tinyfd_utf8to16(lTheSaveFileName),
            L"w"); /* the UTF-8 filename is converted to UTF-16 to open the
                      file*/
    else
#endif
        lIn = fopen(lTheSaveFileName, "w");

    if (! lIn)
    {
        printf("Can not open this file in write mode");
        return 1;
    }
    fputs("ayy lmao", lIn);
    fclose(lIn);

    return 0;
}