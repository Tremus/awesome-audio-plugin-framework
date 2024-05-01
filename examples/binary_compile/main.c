#include "assets/myfile_bin.h"
#include <stdio.h>
int main()
{
    printf("File:\n%.*s", myfile_len, myfile_buf);
    return 0;
}