# Working with files
In this chapter we are going to discuss how to read and write to files with `Vita`! The functinality presented below can be found in [`system/fileio.h`](../../inc/vita/system/fileio.h)

## Contents
* [Reading from files on one line of code](page3.md#reading-from-files-on-one-line-of-code)
* [Writing to files](page3.md#writing-to-files)

### Reading from files on one line of code
```c
// read plain data
vt_str_t *s = vt_file_read("other/myfile.txt");

// read in binary mode
vt_str_t *s = vt_file_readb("other/myfile.txt");

// do you thing ...

vt_str_destroy(s); // free data
```

### Writing to files
Write data in one line of code:
```c
// string to write to file
const char *filename = "myfile.txt";
const char *data_buffer = "hello, world\n";

// write to file
vt_file_write(filename, data_buffer);
vt_file_writeln(filename, data_buffer);

// append at the end of a file
vt_file_append(filename, data_buffer);
vt_file_appendln(filename, data_buffer);

// free resources
vt_str_destroy(sbuf);
```

**[ [Back](page2.md) | [Next](page4.md) ]**
