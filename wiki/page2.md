# Using Vita containers
In this chapter we are going to discuss how to use `Vita` containers. Currently, `Vita` support 4 container types:

```
* str_t     // similar to std::string
* vec_t     // similar to std::vector or std::array
* mat_t     // similar to 2d array or matrix
* plist_t   // an array of pointers
```

Every container is an alias of [`BaseContainerType`](../inc/vita/core/core.h#L115) struct.

## Contents
* [Using `str_t` strings](page2.md#using-str_t-strings)
* [Dynamic arrays with `vec_t`](page2.md#dynamic-arrays-with-vec_t)
* [Matrices and 2d arrays with `mat_t` types](page2.md#matrices-and-2d-arrays-with-mat_t-types)
* [A list of pointers with `plist_t`](page2.md#a-list-of-pointers-with-plist_t)

### Using `str_t` strings
`Vita` has a lot of string functions available to the user. `str_fmt, str_split, str_strip` just to name a few. To read more about available functions, check out the [str_t](../inc/vita/container/str.h) header file. It also handles `'\0'` internally, so you don't need to worry about it.

### Creating strings
```c
// creates a string and sets its value to "hello, world!"
str_t *msg = str("hello, world!");

// the same as above, but in 2 steps:
str_t *msg = strn(10);                      // 1. creates a string with length 10
str_set(msg, "hello, world!");              // 2. sets its value to "hello, world!"

// almost the same as above 
str_t *msg = strn_empty(10);                // creates an empty string with length of 0 and capacity of 10
str_append(msg, "hello, world!");           // appends "hello, world!"
str_appendf(msg, "%s!", "hello, world");    // appends "hello, world!"

// the same as above, but on one go with formatting 
// passing NULL here will allocate a new string instance
const char *myVar = "world";
str_t *msg = str_fmt(NULL, "hello, %s!", myVar);

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

```c
// create/destroy a vector instance
vec_t *v = vec_create(10, sizeof(int32_t));
vec_destroy(v);

// get vector info
const size_t v_length = vec_len(v);
const size_t v_capacity = vec_capacity(v);
const size_t hasSpace = vec_has_space(v);
const bool isEmpty = vec_is_empty(v);

// push data
const int32_t var = 33;
vec_push(v, &var);
vec_pushi32(v, 33);

// set data
vec_set(v, &var, 0);    // assign vector[0] = var
vec_seti32(v, 33, 0);   // assign vector[0] = 33

// get data
const int32_t myVal = *(int32_t*)vec_get(v, 0);
const int32_t myVal = vec_geti32(v, 0);

// remove data
const int32_t index = vec_contains(v, &myVal);
if(index >= 0) { // index is -1 if element not found
    vec_remove(v, index, rs_fast);
    /**
     * RemoveStrategy => rs
     *  rs_fast     => order does not matter
     *  rs_stable   => keep order
    */
}

// pop value
vec_pop(v);
const int32_t popVal = *(int32_t*)vec_pop_get(v);

// set length to 0
vec_clear(v);

// foreach looping
vec_apply(v, func); // func(void *ptr, size_t index)
```

For more details, please refer to [vec.h](../inc/vita/container/vec.h) or string [test_vec.c](../tests/src/test_vec.c) files.

### Matrices and 2d arrays with `mat_t` types

```c
const size_t rows = 3;
const size_t cols = 4;

// create/destroy a matrix instance
mat_t *m = mat_create(rows, cols, sizeof(double));
mat_destroy(m);

// get matrix info
const size_t m_rows = mat_rows(m);
const size_t m_cols = mat_cols(m);
const size_t m_size = mat_size(m); // rows * cols

const size_t atRow = 2;
const size_t atCol = 3;
const double value = 25.3476;

// set data
mat_set(m, &value, atRow, atCol);
mat_setd(m, 25.3476, atRow, atCol);

// get data
const double retValue = *(double*)mat_get(m, atRow, atCol);
const double retValue = mat_getd(m, atRow, atCol);

// manipulate data
mat_t *mdup = mat_dup(m);   // copy
mat_resize(m, 5, 5);        // mat_t*, rows, cols
mat_apply(m, func);         // func(void *ptr, size_t row, size_t row)
```

For more details, please refer to [mat.h](../inc/vita/container/mat.h) or string [test_mat.c](../tests/src/test_mat.c) files.

### A list of pointers with `plist_t`

```c
// create/destroy a pointer list instance
plist_t *p = plist_create(5);   // allocate 5 elements
plist_destroy(p);               // frees all elements with free()

char *h = strdup("hello");
char *w = strdup("world");

// add/remove data
plist_push(p, h);
plist_push(p, w);
plist_remove(p, 0, rs_fast);

assert(plist_len(p) == 1);
assert(plist_capacity(p) == 5);
assert(plist_has_space(p) == 4);

// basic operations
plist_shrink(p);
plist_reserve(p, 5);
```

For more details, please refer to [plist.h](../inc/vita/container/plist.h) or string [test_plist.c](../tests/src/test_plist.c) files.
