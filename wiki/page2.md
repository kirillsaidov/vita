# Using Vita containers
In this chapter we are going to discuss how to use `Vita` containers. Currently, `Vita` support 4 container types:

```
* str_t     // similar to std::string
* vec_t     // similar to std::vector or std::array
* mat_t     // similar to 2d array or matrix
* plist_t   // an array of pointers
```

Every container is an alias of [`BaseContainerType`](../inc/vita/core/core.h) struct.

## Contents
* [Using `str_t` strings](https://github.com/kirillsaidov/vita/blob/master/wiki/page2.md#using-str_t-strings)
* [Dynamic arrays with `vec_t`](https://github.com/kirillsaidov/vita/blob/master/wiki/page2.md#dynamic-arrays-with-vec_t)
* [Matrices and 2d arrays with `mat_t` types](https://github.com/kirillsaidov/vita/blob/master/wiki/page2.md#matrices-and-2d-arrays-with-mat_t-types)
* [A list of pointers with `plist_t`](https://github.com/kirillsaidov/vita/blob/master/wiki/page2.md#a-list-of-pointers-with-plist_t)

### Using `str_t` strings
`Vita` has a lot of string functions available to the user. `str_fmt, str_split, str_strip` just to name a few. To read more about available functions, check out the [str_t](../inc/vita/container/str.h) header file. It also handles `'\0'` internally, so you don't need to worry about it.

### Creating strings
```c
// creates a string and sets its value to "hello, world!"
str_t *msg = str("hello, world!");

// the same as above, but in 2 steps:
str_t *msg = strn(10);                      // 1. creates a string with length 10
str_set(msg, "hello, world!");              // 2. sets its value to "hello, world!"

// the same as above 
str_t *msg = strn_empty(10);                // creates an empty string with length of 0 and capacity of 10
str_append(msg, "hello, world!");           // appends "hello, world!"
str_appendf(msg, "%s!", "hello, world");    // appends "hello, world!"

// the same as above, but on one go with formatting 
// passing NULL here will allocate a new string instance
str_t *msg = str_fmt(NULL, "hello, %s", myVar);

// create a copy
str_t *msg_copy = str_dup(msg);

// creating a string from heap allocated memory
str_t *msg_heap_alloced = str_take_ownership(strdup("hello, world"));

// get string info
const size_t s_length = str_len(msg);
const size_t s_capacity = str_capacity(msg);
const size_t freeSpace = str_has_space(msg);
const bool isEmpty = str_is_empty(msg);

// accessing the raw string pointer
const char *z_str = cstr(msg); // !!! don't free it

// free memory
str_free(msg);
str_free(msg_copy);
str_free(msg_heap_alloced);
```

### String operations
```c
// comparing strings
assert(str_equals(cstr(msg), "hello, world"));

// checking if string is a numeric value
const size_t max_check_len = 256;
assert(str_is_numeric("123", max_check_len) == true);
assert(str_is_numeric("99.3", max_check_len) == true);
assert(str_is_numeric("15,7", max_check_len) == false);
assert(str_is_numeric("this is a str 123", max_check_len) == false);

// basic operations
str_reserve(msg, 100);              // reserve 100 chars
str_shrink(msg);                    // shrink to length
str_clear(msg);                     // sets length = 0 and [0] = '\0' 

str_insert(msg, "Oranges", 0);      // inserts at position
str_remove(msg, 4, 3);              // removes 3 chars starting from 4th index

// check substring
assert(str_can_find(msg, "world") == true);

// advanced operations
str_remove_first(msg, "Watermellon");   // removes the first encountered substring
str_remove_last(s_strip, "world");      // removes the last encountered substring
str_remove_all(s_strip, ".");           // removes every substring in a string
str_remove_c(s_strip, ",!.");           // removes every char '.' ',' '!' in a string

str_strip("  hello, world\n   ");       // strips leading and tailing whitespace and control symbols
str_strip_punct(",. \n hello, world!")  // strips leading and tailing punctuation marks + whitespace and control symbols
```

There are many more advanced functions available like `str_starts_with, str_index_of` and `str_capitalize`. For more details, please refer to [str.h](../inc/vita/container/str.h) or string [test_str.c](../tests/src/test_str.c) files.

### Dynamic arrays with `vec_t`


