# File Searching

This uses a slightly modified version of Randy Gauls [cute_files.h](https://github.com/RandyGaul/cute_headers_deprecated/blob/master/cute_files.h) library. Randys version filters out directories directories in `cf_traverse`, however I removed the filter. This way we can do the filtering for files & folders ourselves.