# Using Vita containers
In this chapter we are going to discuss how to use `Vita` containers. Currently, `Vita` support 4 container types:

```
* str_t     // string
* vec_t     // similar to std::string or array
* mat_t     // similar to 2d array, but is meant to be used as a matrix
* plist_t   // array of pointers
```

Every container is an alias of [`BaseContainerType`](../inc/vita/core/core.h) struct.

## Contents
* Using `str_t` strings
* Dynamic arrays with `vec_t`
* Matrices and 2d arrays with `mat_t` types
* Storing pointers in `plist_t`

### Using `str_t` strings
`Vita` has a lot of string functions available to the user. `str_fmt, str_split, str_strip` just to name a few. To read more about available functions, check out the [str_t](../inc/vita/container/str.h) header file.

### Creating strings
```C
// creates a string and sets its value to "hello, world!"
str_t *msg = str("hello, world!");

// creates a string with length 10 ('\0' excluded, handled internally)
str_t *msg = strn(10);

// creates an empty string with length of 0 and capacity of 10 chars
str_t *msg = strn_empty(10);

// creates a formatted string; passing NULL here will allocate a new str_t instance instead of using an existing one
str_t *msg = str_fmt(NULL, "hello, %s", myVar);

// create a copy
str_t *msg_copy = str_dup(msg);

// creating a string from heap allocated memory
str_t *msg_heap_alloced = str_take_ownership(strdup("hello, world"));

// free memory
str_free(msg);
str_free(msg_copy);
str_free(msg_heap_alloced);
```

### Manipulating strings
#### Get string info
```c
// container info
const size_t sLen = str_len(msg);
const size_t sCap = tr_capacity(msg);
const size_t freeSpace = str_has_space(msg);
const bool isEmpty = str_is_empty(msg);

// get a raw c string
const char *z_str = cstr(msg); // !!! don't free it yourself

// some useful functions
assert(str_equals(cstr(msg), "hello, world"));
assert(str_can_find(msg, "world") == true);

const size_t max_check_len = 256;
assert(str_is_numeric(cstr(msg), max_check_len) == false);
```

#### Adding data
```c
// reserve 100 chars
str_reserve(msg, 100);

// shrink to length
str_shrink(msg);

// zero out the string and set length = 0
str_clear(msg);

// working with string values
str_set(msg, "Apples are sweet!");                      // fails if not enough length
str_append(msg, "Watermellon is red...");               // appends the string reserving more memory if needed
str_appendf(msg, "%s %d %s", "I have", 2, "apples.");   // the same as above, but with formatting
str_insert(msg, "Oranges", 0);                         // inserts at position
str_remove(msg, 4, 3);                                  // removes 3 chars starting from 4th index
```

#### String manipulation
```c
str_remove_first(msg, "Watermellon");   // removes the first encountered substring
str_remove_last(s_strip, "world");      // removes the last encountered substring
str_remove_all(s_strip, ".");           // removes every substring in a string

```
