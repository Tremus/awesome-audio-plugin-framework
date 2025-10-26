WIP

Demo of how to generate a stacktrace file, strip symbols from your binary, and locate symbols in your binary

The `.sh` program: - compiles and runs the `.c` program - The C program demos how to generates a stacktrace and log it to a file - creates a `.dSYM`, and strips symbols from the program

Run the script, then run the python program:

`python3 address_lookup.py ./build/make_stacktrace.dSYM stacktrace_<unixtimestamp>.txt`
