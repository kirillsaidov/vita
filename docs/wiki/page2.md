# Using Vita containers
In this chapter we are going to discuss how to use `Vita` containers. Currently, `Vita` support 3 container types:

```
* vt_str_t     // similar to std::string
* vt_vec_t     // similar to std::vector or std::array
* vt_plist_t   // an array of pointers
```

Every container is an alias of [`VitaBaseArrayType`](../../inc/vita/core/core.h#L115) struct.

## Contents
* [Using `vt_str_t` strings](page2.md#using-vt_str_t-strings)
* [Dynamic arrays with `vt_vec_t`](page2.md#dynamic-arrays-with-vt_vec_t)
* [Using `vt_vec_t` type as 2d array](page2.md#using-vt_vec_t-type-as-2d-array)
* [A list of pointers with `vt_plist_t`](page2.md#a-list-of-pointers-with-vt_plist_t)

### Using `vt_str_t` strings
`Vita` has a lot of string functions available to the user. `vt_str_fmt, vt_str_split, vt_str_strip` just to name a few. To read more about available functions, check out the [vt_str_t](../../inc/vita/container/str.h) header file. It also handles `'\0'` internally, so you don't need to worry about it.

### Creating strings
```c
// creates a string and sets its value to "hello, world!"
vt_str_t *msg = vt_str_create("hello, world!");

// the same as above, but in 2 steps:
vt_str_t *msg = vt_str_create_len(10);                      // 1. creates a string with length 10
vt_str_set(msg, "hello, world!");              // 2. sets its value to "hello, world!"

// almost the same as above 
vt_str_t *msg = vt_str_create_capacity(10);                // creates an empty string with length of 0 and capacity of 10
vt_str_append(msg, "hello, world!");           // appends "hello, world!"
vt_str_appendf(msg, "%s!", "hello, world");    // appends "hello, world!"

// the same as above, but on one go with formatting 
// passing NULL here will allocate a new string instance
const char *myVar = "world";
vt_str_t *msg = vt_str_create_fmt(NULL, "hello, %s!", myVar);

// create a copy
vt_str_t *msg_copy = vt_str_dup(msg);

// creating a string from heap allocated memory
vt_str_t *msg_heap_alloced = vt_str_take_ownership(strdup("hello, world"));

// get string info
const size_t s_length = vt_str_len(msg);
const size_t s_capacity = vt_str_capacity(msg);
const size_t freeSpace = vt_str_has_space(msg);
const bool isEmpty = vt_str_is_empty(msg);

// accessing the raw string pointer
const char *z_str = vt_str_z(msg); // !!! don't free it

// free memory
vt_str_destroy(msg);
vt_str_destroy(msg_copy);
vt_str_destroy(msg_heap_alloced);
```

### String operations
```c
// comparing strings
assert(vt_str_equals(vt_str_z(msg), "hello, world"));

// checking if string is a numeric value
const size_t max_check_len = 256;
assert(vt_str_is_numeric("123", max_check_len) == true);
assert(vt_str_is_numeric("99.3", max_check_len) == true);
assert(vt_str_is_numeric("15,7", max_check_len) == false);
assert(vt_str_is_numeric("this is a str 123", max_check_len) == false);

// basic operations
vt_str_reserve(msg, 100);              // reserve 100 chars
vt_str_shrink(msg);                    // shrink to length
vt_str_clear(msg);                     // sets length = 0 and [0] = '\0' 

vt_str_insert(msg, "Oranges", 0);      // inserts at position
vt_str_remove(msg, 4, 3);              // removes 3 chars starting from 4th index

// check substring
assert(vt_str_can_find(msg, "world") == true);

// advanced operations
vt_str_remove_first(msg, "Watermellon");   // removes the first encountered substring
vt_str_remove_last(s_strip, "world");      // removes the last encountered substring
vt_str_remove_all(s_strip, ".");           // removes every substring in a string
vt_str_remove_c(s_strip, ",!.");           // removes every char '.' ',' '!' in a string

vt_str_strip("  hello, world\n   ");       // strips leading and tailing whitespace and control symbols
vt_str_strip_punct(",. \n hello, world!")  // strips leading and tailing punctuation marks + whitespace and control symbols
```

There are many more advanced functions available like `vt_str_starts_with, vt_str_vt_index_of` and `vt_str_capitalize`. For more details, please refer to [str.h](../../inc/vita/container/str.h) or string [test_str.c](../../tests/src/test_str.c) files.

### Dynamic arrays with `vt_vec_t`

```c
// create/destroy a vector instance
vt_vec_t *v = vt_vec_create(10, sizeof(int32_t));
vt_vec_destroy(v);

// get vector info
const size_t v_length = vt_vec_len(v);
const size_t v_capacity = vt_vec_capacity(v);
const size_t hasSpace = vt_vec_has_space(v);
const bool isEmpty = vt_vec_is_empty(v);

// push data
const int32_t var = 33;
vt_vec_push(v, &var);
vt_vec_pushi32(v, 33);

// set data
vt_vec_set(v, &var, 0);    // assign vector[0] = var
vt_vec_seti32(v, 33, 0);   // assign vector[0] = 33

// get data
const int32_t myVal = *(int32_t*)vt_vec_get(v, 0);
const int32_t myVal = vt_vec_geti32(v, 0);

// remove data
const int32_t index = vt_vec_can_find(v, &myVal);
if(index >= 0) { // index is -1 if element not found
    vt_vec_remove(v, index, vt_remove_stategy_fast);
    /**
     * VitaRemoveStrategy => rs
     *  vt_remove_stategy_fast         => order does not matter
     *  vt_remove_stategy_stable    => keep order
    */
}

// pop value
vt_vec_pop(v);
const int32_t popVal = *(int32_t*)vt_vec_pop_get(v);

// set length to 0
vt_vec_clear(v);

// foreach looping
vt_vec_apply(v, func); // func(void *ptr, size_t index)
```

For more details, please refer to [vec.h](../../inc/vita/container/vec.h) or string [test_vec.c](../../tests/src/test_vec.c) files.

### Using `vt_vec_t` type as 2d array

```c
// rows and columns
const size_t r = 5;
const size_t c = 5;

vt_vec_t *vec2d = vt_vec_create(r*c, sizeof(int32_t));

// right now it acts as a list, we need to set its length = r*c
assert(vt_vec_len(vec2d) == 0);
assert(vt_vec_capacity(vec2d) == r*c);

// set vec2d length = r*c
vt_vec_resize(vec2d, r*c);
assert(vt_vec_len(vec2d) == r*c);
assert(vt_vec_capacity(vec2d) == r*c);
assert(vt_vec_has_space(vec2d) == 0);

// fill with 1s to create an Identity matrix
vt_vec_seti32(vec2d, 1, vt_index_2d_to_1d(0, 0, w));
vt_vec_seti32(vec2d, 1, vt_index_2d_to_1d(1, 1, w));
vt_vec_seti32(vec2d, 1, vt_index_2d_to_1d(2, 2, w));
vt_vec_seti32(vec2d, 1, vt_index_2d_to_1d(3, 3, w));
vt_vec_seti32(vec2d, 1, vt_index_2d_to_1d(4, 4, w));

// check values along the diagonal of the Identity matrix
assert(vt_vec_geti32(vec2d, 0) == 1);
assert(vt_vec_geti32(vec2d, 6) == 1);
assert(vt_vec_geti32(vec2d, 12) == 1);
assert(vt_vec_geti32(vec2d, 18) == 1);
assert(vt_vec_geti32(vec2d, 24) == 1);

// free resources
vt_vec_destroy(vec2d);
```

For more details, please refer to [vec.h](../../inc/vita/container/vec.h) or string [test_vec.c](../../tests/src/test_vec.c) files.

### A list of pointers with `vt_plist_t`

```c
// create/destroy a pointer list instance
vt_plist_t *p = vt_plist_create(5);   // allocate 5 elements
vt_plist_destroy(p);               // frees all elements with free()

char *h = strdup("hello");
char *w = strdup("world");

// add/remove data
vt_plist_push(p, h);
vt_plist_push(p, w);
vt_plist_remove(p, 0, vt_remove_stategy_fast);

assert(vt_plist_len(p) == 1);
assert(vt_plist_capacity(p) == 5);
assert(vt_plist_has_space(p) == 4);

// basic operations
vt_plist_shrink(p);
vt_plist_reserve(p, 5);
```

For more details, please refer to [plist.h](../../inc/vita/container/plist.h) or string [test_plist.c](../../tests/src/test_plist.c) files.
