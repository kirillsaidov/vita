#ifndef VITA_STR_H
#define VITA_STR_H

/** VITA_STR MODULE (dynamic string)
    - str_make_on_stack
    - str
    - str_fmt
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
    - str_remove_all
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
    - str_apply
    - str_is_numeric
    - str_capitalize
    - str_index_of
*/

#include <ctype.h>
#include <stdarg.h>
#include "../core/core.h"
#include "../util/debug.h"
#include "plist.h"

// see core/core.h for definition
typedef struct BaseContainerType str_t;

/** Creates a static string with length
    @param z raw C string
    @returns str_t
*/
str_t str_make_on_stack(const char *const z);

/** Creates a new dynamic string from a raw C string (allocates additional memory for '\0')
    @param z raw C string
    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *str(const char *z);

/** Creates a formatted dynamic string (it is initialized to zero before usage)
    @param s str_t instance; if `NULL` is passed, allocates
    @param fmt formatting
    @param ... additional arguments

    @returns `str_t*` upon success, `NULL` otherwise
*/
extern str_t *str_fmt(str_t *s, const char *const fmt, ...);

/** Creates a dynamic string of specified length and fills it with zeros (allocates additional memory for '\0')
    @param n number of elements
    @returns `str_t*` upon success, `NULL` otherwise

    @note:
        Its length is n, thus, appending to it, will add a string to the end (after n elements). 
        It won't start appending from the begining. Use `str_set` for that. 
*/
extern str_t *strn(const size_t n);

/** Creates an empty dynamic string of specified capacity and fills it with zeros (allocates additional memory for '\0')
    @param n number of elements
    @returns `str_t*` upon success, `NULL` otherwise

    @note:
        Its length is 0, capacity is n. Appending to it will start from the begining. 
        `str_set` won't work this time, because not enough space (length 0).
*/
extern str_t *strn_empty(const size_t n);

/** Duplicates and returns a new dynamic string
    @param s str_t instance
    @returns str_t* instance upon success, `NULL` otherwise
*/
extern str_t *str_dup(const str_t *const s);

/** Takes ownership of an allocated string instead of allocating memory itself
    @param z a raw C string allocated on heap
    @returns str_t* instance upon success, `NULL` otherwise
*/
extern str_t *str_take_ownership(const char *const z);

/** Frees the str instance
    @param s str_t instance
*/
extern void str_free(str_t *s);

/** Returns a raw immutable C string
    @param s str_t string
    @returns raw C string upon success, `NULL` otherwise
*/
extern const char *cstr(const str_t *const s);

/** Returns str_t length
    @param s str_t instance
    @returns str_t length
*/
extern size_t str_len(const str_t *const s);

/** Returns str_t capacity
    @param s str_t instance
    @returns str_t capacity
*/
extern size_t str_capacity(const str_t *const s);

/** Returns available space before new allocation is required
    @param s str_t instance
    @returns free space (capacity - length)
*/
extern size_t str_has_space(const str_t *const s);

/** Checks if string is emtpy ("")
    @param s str_t instance
    @returns `true` if length == 0
*/
extern bool str_is_empty(const str_t *const s);

/** Shrinks str_t capacity to its length
    @param s str_t instance
    @returns enum VitaError
*/
extern enum VitaError str_shrink(str_t *const s);

/** Clears the str_t (sets length to 0)
    @param s str_t instance
    @returns enum VitaError
*/
extern enum VitaError str_clear(str_t *const s);

/** Reserves memory for str_t
    @param s str_t instance
    @param n how many elements to reserve

    @returns enum VitaError
*/
extern enum VitaError str_reserve(str_t *const s, const size_t n);

/** Assigns a new raw C string to str_t
    @param s str_t instance
    @param z raw C string

    @returns enum VitaError
*/
extern enum VitaError str_set(str_t *const s, const char *z);

/** Assigns n characters of raw C string to str_t
    @param s str_t instance
    @param z raw C string
    @param n number of characters

    @returns enum VitaError
*/
extern enum VitaError str_set_n(str_t *const s, const char *z, const size_t n);

/** Appends a raw C string at the end of str_t
    @param s str_t instance
    @param z raw C string

    @returns enum VitaError
*/
extern enum VitaError str_append(str_t *const s, const char *z);

/** Appends a formatted raw C string at the end of str_t
    @param s str_t instance
    @param fmt format

    @returns enum VitaError
*/
extern enum VitaError str_appendf(str_t *const s, const char *const fmt, ...);

/** Appends n characters of raw C string at the end of str_t
    @param s str_t instance
    @param z raw C string
    @param n number of characters

    @returns enum VitaError
*/
extern enum VitaError str_append_n(str_t *const s, const char *z, const size_t n);

/** Inserts a raw C string into str_t starting at the specified index
    @param s str_t instance
    @param z raw C string
    @param at start at index (including `at`)

    @returns enum VitaError
*/
extern enum VitaError str_insert(str_t *const s, const char *z, const size_t at);

/** Removes n chars from str_t, starting from the specified index
    @param s str_t instance
    @param from start from index
    @param n number of elements to remove after `from`

    @returns enum VitaError
*/
extern enum VitaError str_remove(str_t *const s, const size_t from, size_t n);

/** Removes the first encountered substring from str_t
    @param s str_t instance
    @param z raw C string

    @returns enum VitaError
*/
extern enum VitaError str_remove_first(str_t *const s, const char *z);

/** Removes the last encountered substring from str_t
    @param s str_t instance
    @param z raw C string

    @returns enum VitaError
*/
extern enum VitaError str_remove_last(str_t *s, const char *const z);

/** Removes all instances of encountered substring from str_t
    @param s str_t instance
    @param z raw C string

    @returns enum VitaError
*/
extern enum VitaError str_remove_all(str_t *const s, const char *z);

/** Removes all encountered characters specified by the user from str_t
    @param s str_t instance
    @param c characters to remove one after another: "\\n ," => remove new line, whitespace, comma

    @returns enum VitaError
*/
extern enum VitaError str_remove_c(str_t *const s, const char *const c);

/** Strips leading and tailing whitespace and control symbols
    @param s str_t instance
    @returns enum VitaError
*/
extern enum VitaError str_strip(str_t *const s);

/** Strips leading and tailing punctuation marks + whitespace and control symbols
    @param s str_t instance
    @returns enum VitaError
*/
enum VitaError str_strip_punct(str_t *const s);

/** Strips leading and tailing characters specified by the user
    @param s str_t instance
    @param c characters to strip one after another: "\n ," => strip new line, whitespace, comma

    @returns enum VitaError
*/
enum VitaError str_strip_c(str_t *const s, const char *const c);

/** Find a substring
    @param s str_t instance
    @param z raw C string

    @returns pointer to the begining of a substring in a string
*/
extern const char *str_find(const char *const z, const char *csub);

/** Checks if str_t contains a substring
    @param s str_t instance
    @param z raw C string

    @returns number of substring instances in str_t
*/
extern size_t str_can_find(const str_t *const s, const char *z);

/** Splits a string given a separator into substrings
    @param p plist_t instance, if `NULL` allocates
    @param s str_t instance
    @param sep seperator string

    @returns plist_t of str_t, `NULL` upon failure
*/
extern plist_t *str_split(plist_t *ps, const str_t *const s, const char *const sep);

/** Pops off the first part of the string before the separator
    @param sr str_t instance where the result will be saved, if NULL is passed, it's allocated
    @param s str_t instance
    @param sep seperator string

    @returns str_t, `NULL` upon failure
*/
extern str_t *str_pop_get_first(str_t *sr, str_t *const s, const char *const sep);

/** Pops off the last part of the string after the separator
    @param sr str_t instance where the result will be saved, if NULL is passed, it's allocated
    @param s str_t instance
    @param sep seperator string

    @returns str_t, `NULL` upon failure
*/
extern str_t *str_pop_get_last(str_t *sr, str_t *const s, const char *const sep);

/** Checks if two raw C strings are the same
    @param z1 raw C string
    @param z2 raw C string

    @returns `true` if z1 == z2
*/
extern bool str_equals(const char *const z1, const char *const z2);

/** Checks if a raw C string starts with a substring
    @param z raw C string
    @param sub raw C substring

    @returns `true` if z starts with sub
*/
extern bool str_starts_with(const char *const z, const char *const sub);

/** Checks if a raw C string ends with a substring
    @param z raw C string
    @param sub raw C substring

    @returns `true` if z ends with sub
*/
extern bool str_ends_with(const char *const z, const char *const sub);

/** Applies a user specified function upon each char
    @param s str_t
    @param func function to execute upon each element: func(char pointer, for loop index) 
*/
extern void str_apply(const str_t *const s, void (*func)(char*, size_t));

/** Checks if the entire string is a number
    @param z raw c string
    @param max_len max len to top checking (internally it uses strnlen)

    @returns true upon z being a number
*/
extern bool str_is_numeric(const char *const z, const size_t max_len);

/** Capitalizes a string
    @param s str_t
*/
extern void str_capitalize(str_t *const s);

/** Returns the first occurance of char in string
    @param s str_t
    @param z character 

    @returns position of z in str_t upon success, -1 upon failure
*/
extern int64_t str_index_of(const str_t *const s, const char z);

#endif // VITA_STR_H
