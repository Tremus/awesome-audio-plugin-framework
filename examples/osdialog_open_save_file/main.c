#include <osdialog.h>

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#define snwprintf               _snwprintf
#include <osdialog_win.c>
#elif defined(__APPLE__)
#include <osdialog_mac.m>
#endif

#include <osdialog.c>

int main()
{
    char* filename = osdialog_file(OSDIALOG_OPEN, NULL, NULL, NULL);
    if (filename != NULL)
    {
        printf("You selected the file %s with Open dialog\n", filename);
        OSDIALOG_FREE(filename);
    }
    else
        printf("Failed Open dialog...\n");

    filename = osdialog_file(OSDIALOG_SAVE, NULL, NULL, NULL);
    if (filename != NULL)
    {
        printf("You selected the file %s with Save dialog\n", filename);
        OSDIALOG_FREE(filename);
    }
    else
        printf("Failed Save dialog...\n");
    return filename == NULL;
}