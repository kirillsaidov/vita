# Data structures
In this chapter we are going to discuss how to use `Vita` containers. Currently, `Vita` support 3 container types:

```
* vt_str_t     // similar to std::string
* vt_vec_t     // similar to std::vector or std::array
* vt_plist_t   // an array of pointers
```

Every container is an alias of [`VitaBaseArrayType`](../../inc/vita/core/core.h#L133) struct.

## Contents
* [Using `vt_str_t` strings](page2.md#using-vt_str_t-strings)
* [Dynamic arrays with `vt_vec_t`](page2.md#dynamic-arrays-with-vt_vec_t)
* [Imitating 2d arrays with `vt_vec_t`](page2.md#imitating-2d-arrays-with-vt_vec_t)
* [A list of pointers with `vt_plist_t`](page2.md#a-list-of-pointers-with-vt_plist_t)

### Using `vt_str_t` strings
`Vita` has a lot of string functions available to the user. `vt_str_starts_with, vt_str_split, vt_str_strip` just to name a few. To read more about available functions, check out the [vt_str_t](../../inc/vita/container/str.h) header file. It also handles `'\0'` internally, so you don't need to worry about it.

### Creating strings
```c
// create static string (non-modifiable, pointer-length pair)
const vt_str_t str = vt_str_create_static("hello, world!");

// creates a dynamic string and sets its value to "hello, world!"
vt_str_t *str = vt_str_create("hello, world!", alloctr); // if alloctr == NULL, use plain calloc/free

// the same as above, but in 2 steps:
vt_str_t *str = vt_str_create_len(10, alloctr);          // 1. creates a string with length 10
vt_str_set(str, "hello, world!");                        // 2. sets its value to "hello, world!"

// almost the same as above 
vt_str_t *msg = vt_str_create_capacity(32);              // creates an empty string with length of 0 and capacity of 32
vt_str_append(str, "hello, world!");                     // appends "hello, world!"
vt_str_appendf(str, "%s!", "hello, world");              // appends "hello, world!"

// create a copy
vt_str_t *str_copy = vt_str_dup(str, alloctr);           // if alloctr == NULL, use plain calloc/free

/* taking memory ownership from custom allocated block of data
    if alloctr == NULL, assumes plain calloc/free were used to allocate this data,
    otherwise give it the allocator that was used.
*/
vt_str_t *str_heap_alloced = vt_str_take_ownership(strdup("hello, world"), alloctr); 

// get string info
const size_t str_length = vt_str_len(msg);
const size_t str_capacity = vt_str_capacity(msg);
const size_t str_freeSpace = vt_str_has_space(msg);
const bool str_isEmpty = vt_str_is_empty(msg);

// accessing the raw string pointer
const char *z_str = vt_str_z(msg); // !!! don't free it

// free memory
vt_str_destroy(str);
vt_str_destroy(str_copy);
vt_str_destroy(str_heap_alloced);
```

### String operations
```c
// comparing strings
assert(vt_str_equals(vt_str_z(str), "hello, world"));

// checking if string is a numeric value
const size_t max_check_len = 256;
assert(vt_str_is_numeric("123", max_check_len) == true);
assert(vt_str_is_numeric("99.3", max_check_len) == true);
assert(vt_str_is_numeric("15,7", max_check_len) == false);
assert(vt_str_is_numeric("this is a str 123", max_check_len) == false);

// basic operations
vt_str_reserve(str, 100);              // reserve 100 chars
vt_str_shrink(str);                    // shrink to length
vt_str_clear(str);                     // sets length = 0 and [0] = '\0' 

vt_str_insert(str, "Oranges", 0);      // inserts at position
vt_str_remove(str, 4, 3);              // removes 3 chars starting from 4th index

// check substring
assert(vt_str_can_find(str, "world") == true);

// advanced operations
vt_str_remove_first(str, "world");  // removes the first encountered substring
vt_str_remove_last(str, "world");   // removes the last encountered substring
vt_str_remove_all(str, ".");        // removes every substrings in a string
vt_str_remove_c(str, ",!.");        // removes every char '.' ',' '!' in a string

vt_str_strip("  hello, world\n   ");        // strips leading and tailing whitespace and control symbols
vt_str_strip_punct(",. \n hello, world!");  // strips leading and tailing punctuation marks + whitespace and control symbols
```

There are many more advanced functions available like `vt_str_starts_with, vt_str_vt_index_of` and `vt_str_capitalize`. For more details, please refer to [str.h](../../inc/vita/container/str.h) or string [test_str.c](../../tests/src/test_str.c) files.

### Dynamic arrays with `vt_vec_t`
// TODO:
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
    vt_vec_remove(v, index, VT_REMOVE_STRATEGY_FAST);
    /**
     * VitaRemoveStrategy => rs
     *  VT_REMOVE_STRATEGY_FAST         => order does not matter
     *  VT_REMOVE_STRATEGY_STABLE    => keep order
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

### Imitating 2d arrays with `vt_vec_t`

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
vt_plist_remove(p, 0, VT_REMOVE_STRATEGY_FAST);

assert(vt_plist_len(p) == 1);
assert(vt_plist_capacity(p) == 5);
assert(vt_plist_has_space(p) == 4);

// basic operations
vt_plist_shrink(p);
vt_plist_reserve(p, 5);
```

For more details, please refer to [plist.h](../../inc/vita/container/plist.h) or string [test_plist.c](../../tests/src/test_plist.c) files.

**[ [Back](page1.md) | [Next](page3.md) ]**
