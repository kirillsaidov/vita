# Working with files
In this chapter we are going to discuss how to read and write to files with `Vita`! The functinality presented below can be found in [`system/fileio.h`](../../inc/vita/system/fileio.h)

## Contents
* [Reading from files](page3.md#reading-from-files)
* [Writing to files](page3.md#writing-to-files)

### Reading from files
```c
#include "vita/system/fileio.h"

// read plain data
vt_str_t *str = vt_file_read("other/myfile.txt", alloctr);  // if alloctr == NULL, uses plain calloc/free

// read in binary mode
vt_str_t *str = vt_file_readb("other/myfile.txt", alloctr); // ditto

// do you thing ...

vt_str_destroy(str); // free data
```

### Writing to files
Write data in one line of code:
```c
// string to write to file
const char *filename = "myfile.txt";
const char *data_buffer = "hello, world\n";

// write to file
vt_file_write(filename, data_buffer);
vt_file_writef(filename, "%s\n", data_buffer);
vt_file_writeln(filename, data_buffer);
vt_file_writefln(filename, "%s", data_buffer);

// append at the end of a file
vt_file_append(filename, data_buffer);
vt_file_appendln(filename, data_buffer);
```

For more details, please refer to [test_fileio.c](../../tests/src/test_fileio.c) files.

**[ [Back](page2.md) | [Next](page4.md) ]**
