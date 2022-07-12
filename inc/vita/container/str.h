#ifndef VITA_STR_H
#define VITA_STR_H

/** VITA_STR MODULE (dynamic string)
    - str_make_on_stack
    - str
    - strn
    - str_dup
    - str_take_ownership
    - str_free
    - cstr
    - str_len
    - str_capacity
    - str_has_space
    - str_is_empty
    - str_shrink
    - str_clear
    - str_reserve
    - str_set
    - str_set_n
    - str_append
    - str_appendf
    - str_append_n
    - str_insert
    - str_remove
    - str_remove_first
    - str_remove_c
    - str_strip
    - str_strip_punct
    - str_strip_c
    - str_find
    - str_can_find
    - str_split
    - str_pop_get_first
    - str_pop_get_last
    - str_equals
    - str_starts_with
    - str_ends_with
*/

#include <ctype.h>

#include "../core/core.h"
#include "plist.h"

// see core/core.h for definition
typedef struct BaseArrayType str_t;

/**
Creates a static string with length

Params:
    cs = raw C string

Returns: const str_t
*/
const str_t str_make_on_stack(const char *const cs);

/**
Creates a new dynamic string from a raw C string (allocates additional memory for '\0')

Params:
    cs = raw C string

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *str(const char *cs);

/**
Creates an empty dynamic string of specified size (allocates additional memory for '\0')

Params:
    n = number of elements

Returns: `str_t*` upon success, `NULL` otherwise
*/
extern str_t *strn(const size_t n);

/**
Duplicates and returns a new dynamic string

Params:
    s = str_t instance

Returns: str_t* instance upon success, `NULL` otherwise
*/
extern str_t *str_dup(const str_t *const s);

/**
Takes ownership of an allocated string instead of allocating memory itself

Params:
    cs = a raw C string allocated on heap

Returns: str_t* instance upon success, `NULL` otherwise
*/
extern str_t *str_take_ownership(const char *const cs);

/**
Frees the str instance

Params:
    s = str_t instance
*/
extern void str_free(str_t *s);

/**
Returns a raw immutable C string

Params:
    s = str_t string

Returns: raw C string upon success, `NULL` otherwise
*/
extern const char *cstr(const str_t *const s);

/**
Returns str_t length

Params:
    s = str_t instance

Returns: str_t length
*/
extern size_t str_len(const str_t *const s);

/**
Returns str_t capacity

Params:
    s = str_t instance

Returns: str_t capacity
*/
extern size_t str_capacity(const str_t *const s);

/**
Returns available space before new allocation is required

Params:
    s = str_t instance

Returns: free space (capacity - length)
*/
extern size_t str_has_space(const str_t *const s);

/**
Checks if string is emtpy ("")

Params:
    s = str_t instance

Returns: `true` if length == 0
*/
extern bool str_is_empty(const str_t *const s);

/**
Shrinks str_t capacity to its length

Params:
    s = str_t instance

Returns: enum VitaError
*/
extern enum VitaError str_shrink(str_t *const s);

/**
Clears the str_t (sets length to 0)

Params:
    s = str_t instance

Returns: enum VitaError
*/
extern enum VitaError str_clear(str_t *const s);

/**
Reserves memory for str_t

Params:
    s = str_t instance
    n = how many elements to reserve

Returns: enum VitaError
*/
extern enum VitaError str_reserve(str_t *const s, const size_t n);

/**
Assigns a new raw C string to str_t

Params:
    s = str_t instance
    cs = raw C string

Returns: enum VitaError
*/
extern enum VitaError str_set(str_t *const s, const char *cs);

/**
Assigns n characters of raw C string to str_t

Params:
    s = str_t instance
    cs = raw C string
    n = number of characters

Returns: enum VitaError
*/
extern enum VitaError str_set_n(str_t *const s, const char *cs, const size_t n);

/**
Appends a raw C string at the end of str_t

Params:
    s = str_t instance
    cs = raw C string

Returns: enum VitaError
*/
extern enum VitaError str_append(str_t *const s, const char *cs);

/**
Appends a formatted raw C string at the end of str_t

Params:
    s = str_t instance
    fmt = format

Returns: enum VitaError
*/
extern enum VitaError str_appendf(str_t *const s, const char *const cs_fmt, ...);

/**
Appends n characters of raw C string at the end of str_t

Params:
    s = str_t instance
    cs = raw C string
    n = number of characters

Returns: enum VitaError
*/
extern enum VitaError str_append_n(str_t *const s, const char *cs, const size_t n);

/**
Inserts a raw C string into str_t starting at the specified index

Params:
    s = str_t instance
    cs = raw C string
    at = start at index (including `at`)

Returns: enum VitaError
*/
extern enum VitaError str_insert(str_t *const s, const char *cs, const size_t at);

/**
Removes n chars from str_t, starting from the specified index

Params:
    s = str_t instance
    from = start from index
    n = number of elements to remove after `from`

Returns: enum VitaError
*/
extern enum VitaError str_remove(str_t *const s, const size_t from, size_t n);

/**
Removes the first encountered substring from str_t

Params:
    s = str_t instance
    cs = raw C string

Returns: enum VitaError
*/
extern enum VitaError str_remove_first(str_t *const s, const char *cs);

/**
Removes all encountered characters specified by the user from str_t

Params:
    s = str_t instance
    c = characters to remove one after another: "\n ," => remove new line, whitespace, comma

Returns: enum VitaError
*/
extern enum VitaError str_remove_c(str_t *const s, const char *const c);

/**
Strips leading and tailing whitespace and control symbols

Params:
    s = str_t instance

Returns: enum VitaError
*/
extern enum VitaError str_strip(str_t *const s);

/**
Strips leading and tailing punctuation marks + whitespace and control symbols

Params:
    s = str_t instance

Returns: enum VitaError
*/
enum VitaError str_strip_punct(str_t *const s);

/**
Strips leading and tailing characters specified by the user

Params:
    s = str_t instance
    c = characters to strip one after another: "\n ," => strip new line, whitespace, comma

Returns: enum VitaError
*/
enum VitaError str_strip_c(str_t *const s, const char *const c);

/**
Find a substring

Params:
    s = str_t instance
    cs = raw C string

Returns: pointer to the begining of a substring in a string
*/
extern const char *str_find(const char *const cs, const char *csub);

/**
Checks if str_t contains a substring

Params:
    s = str_t instance
    cs = raw C string

Returns: number of substring instances in str_t
*/
extern size_t str_can_find(const str_t *const s, const char *cs);

/**
Splits a string given a separator into substrings

Params:
    p = plist_t instance, if `NULL` allocates
    s = str_t instance
    sep = seperator string

Returns: plist_t of str_t, `NULL` upon failure
*/
extern plist_t *str_split(plist_t *ps, const str_t *const s, const char *const sep);

/**
Pops off the first part of the string before the separator

Params:
    sr = str_t instance where the result will be saved, if NULL is passed, it's allocated
    s = str_t instance
    sep = seperator string

Returns: str_t, `NULL` upon failure
*/
extern str_t *str_pop_get_first(str_t *sr, str_t *const s, const char *const sep);

/**
Pops off the last part of the string after the separator

Params:
    sr = str_t instance where the result will be saved, if NULL is passed, it's allocated
    s = str_t instance
    sep = seperator string

Returns: str_t, `NULL` upon failure
*/
extern str_t *str_pop_get_last(str_t *sr, str_t *const s, const char *const sep);

/**
Checks if two raw C strings are the same

Params:
    cs1 = raw C string
    cs2 = raw C string

Returns: `true` if cs1 == cs2
*/
extern bool str_equals(const char *const cs1, const char *const cs2);

/**
Checks if a raw C string starts with a substring

Params:
    cs = raw C string
    cs_sub = raw C substring

Returns: `true` if cs starts with cs_sub
*/
extern bool str_starts_with(const char *const cs, const char *const cs_sub);

/**
Checks if a raw C string ends with a substring

Params:
    cs = raw C string
    cs_sub = raw C substring

Returns: `true` if cs ends with cs_sub
*/
extern bool str_ends_with(const char *const cs, const char *const cs_sub);

#endif // VITA_STR_H
