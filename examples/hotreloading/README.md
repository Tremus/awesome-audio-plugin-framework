# 🔥 reloading

Simple file watcher on a seperate thread that sets a flag when a file is changed.
The main thread checks the flag. If the flag is set, it unloads the dynamic library, rebuilds the project, reloads the library & exported library function, then calls the exported library function (prints hello to `stderr`)

To achieve all this involves a bunch of platform specific code so I had to create seperate files for each implementation.
