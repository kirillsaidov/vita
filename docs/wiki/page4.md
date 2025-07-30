# Working with paths and directories
`Vita` provides extensive functinality to work with paths and directories. Read more below or check out [`system/path.h`](../../inc/vita/system/path.h).

## Contents
* [Path manipulation](page4.md#path-manipulation)
* [Create, delete, move](page4.md#create-delete-move)
* [Listing directory contents](page4.md#listing-directory-contents)
* [Checking directory info](page4.md#checking-directory-info)

### Path manipulation
```c
#include "vita/system/path.h"

// path concatenation
// If NULL is passed, allocates a new instance, otherwise the said instance is used and returned
vt_str_t *my_path = vt_path_build(NULL, 2, "photos", "mountains.jpg"); 
assert(vt_str_equals_z(vt_str_z(my_path), "photos/mountains.jpg"));

// extracting basename from path
// If my_path is NULL, allocates a new instance, otherwise the said instance is used and returned
my_path = vt_path_basename(my_path, "photos/mountains.jpg");
assert(vt_str_equals_z(vt_str_z(my_path), "mountains.jpg"));

// expands tilda `~` to HOMEPATH both on Unix and Windows
vt_str_t *path_expanded = vt_path_expand_tilda("~/media/dev", alloctr);  // if alloctr == NULL, uses plain calloc/free
assert(vt_str_equals_z(vt_str_z(path_expanded), "/home/userX/media/dev"));

// get your EXE path
vt_str_t *selfpath = vt_path_get_this_exe_location(alloctr);             // if alloctr == NULL, uses plain calloc/free
printf("%s\n", vt_str_z(selfpath)); // prints "/home/userX/media/Vita/tests/bin/test_path"

// free resources
vt_str_destroy(my_path); 
vt_str_destroy(path_expanded);
vt_str_destroy(selfpath);
```

### Create, delete, move
```c
/**
 * All functions below return a boolean value:
 *  `true` upon success
 *  `false` upon failure
*/

// --- Example ---
// create path
bool success = vt_path_mkdir(mypath);
if (!success) {
    // handle error
    // ...
}

// ditto
vt_path_mkdir_parents(mypath);

// rename a file or directory
vt_path_rename(mypath, "newname");

// delete a file
vt_path_remove(mypath);

// delete directory
vt_path_rmdir(mypath);
vt_path_rmdir_recurse(mypath);
```

### Listing directory contents
```c
// If NULL is passed, allocates a new instance, otherwise the passed instance is used and returned
vt_plist_t *pdir = vt_path_dir_list(NULL, "media/dev", true); // `true`: ignore dot.files: .vim, .gitignore, etc...
assert(vt_plist_len(pdir) == FILES_IN_DIR);

// do something
vt_plist_apply(pdir, your_func);

// or this
const size_t len = vt_plist_len(pdir);
for (size_t i = 0; i < len; i++) {
    vt_str_t *path = (vt_str_t*)vt_plist_get(pdir, i);

    // do something with "path"
    // ...
}

// free resources
vt_plist_apply(pdir, free_str);    // void free_str(void *ptr, size_t index) { vt_str_destroy(ptr); }
vt_plist_destroy(pdir);            // destroy `vt_plist_t` itself
```

### Checking directory info
```c
const char *zpath = "media/dev";

assert(vt_path_exists(zpath));
assert(vt_path_is_dir(zpath));
assert(!vt_path_is_file(zpath));

const int64_t file_size = vt_path_get_file_size("myfile.txt");
if (file_size < 0) {
    // error ...
}

// remove the last path segment
vt_path_pop(zpath);
assert(vt_str_equals_z(zpath, "media"));

// validate path for correct VT_PATH_SEPARATOR and fix it
zpath = "media\\dev\\main.c";
vt_path_validate(zpath);
assert(vt_str_equals_z(zpath, "media/dev/main.c")); // if on unix
```

For more details, please refer to [test_fileio.c](../../tests/src/test_path.c) files.

**[ [Back](page3.md) | [Next](page5.md) ]**
