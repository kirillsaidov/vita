#ifndef VITA_STR_H
#define VITA_STR_H

/** VITA_STR MODULE (dynamic string)
    - vt_str_create_static
    - vt_str_create
    - vt_str_create_len
    - vt_str_create_capacity
    - vt_str_destroy
    - vt_str_dup
    - vt_str_take_ownership
    - vt_str_z
    - vt_str_len
    - vt_str_capacity
    - vt_str_has_space
    - vt_str_is_empty
    - vt_str_validate_len
    - vt_str_shrink
    - vt_str_clear
    - vt_str_reserve
    - vt_str_resize
    - vt_str_set
    - vt_str_set_n
    - vt_str_append
    - vt_str_appendf
    - vt_str_append_n
    - vt_str_insert
    - vt_str_remove
    - vt_str_remove_first
    - vt_str_remove_all
    - vt_str_remove_c
    - vt_str_strip
    - vt_str_strip_punct
    - vt_str_strip_c
    - vt_str_find
    - vt_str_can_find
    - vt_str_split
    - vt_str_split_between
    - vt_str_pop_get_first
    - vt_str_pop_get_last
    - vt_str_equals
    - vt_str_starts_with
    - vt_str_ends_with
    - vt_str_apply
    - vt_str_is_numeric
    - vt_str_capitalize
    - vt_str_index_of
*/

#include <ctype.h>
#include <stdarg.h>
#include "common.h"
#include "../container/plist.h"

// see core/core.h for definition
typedef struct VitaBaseArrayType vt_str_t;

/** Creates a static string with length
    @param z raw C string
    @returns vt_str_t
*/
vt_str_t vt_str_create_static(const char *const z);

/** Creates a new dynamic string from a raw C string (allocates additional memory for '\0')
    @param z raw C string
    @param alloctr allocator instance

    @returns `vt_str_t*` upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then vt_calloc/realloc/free is used.
*/
extern vt_str_t *vt_str_create(const char *const z, const struct VitaBaseAllocatorType *const alloctr);

/** Creates a dynamic string of specified length and fills it with zeros (allocates additional memory for '\0')
    @param n number of elements
    @param alloctr allocator instance

    @returns `vt_str_t*` upon success, `NULL` otherwise

    @note
        Its length is n, thus, appending to it, will add a string to the end (after n elements). 
        It won't start appending from the begining. Use `vt_str_set` for that. 
        If `alloctr = NULL` is specified, then vt_calloc/realloc/free is used.
*/
extern vt_str_t *vt_str_create_len(const size_t n, const struct VitaBaseAllocatorType *const alloctr);

/** Creates an empty dynamic string of specified capacity and fills it with zeros (allocates additional memory for '\0')
    @param n number of elements
    @param alloctr allocator instance

    @returns `vt_str_t*` upon success, `NULL` otherwise

    @note
        Its length is 0, capacity is n. Appending to it will start from the begining. 
        `vt_str_set` won't work this time, because not enough space (length 0).
        If `alloctr = NULL` is specified, then vt_calloc/realloc/free is used.
*/
extern vt_str_t *vt_str_create_capacity(const size_t n, const struct VitaBaseAllocatorType *const alloctr);

/** Frees the str instance
    @param s vt_str_t instance
*/
extern void vt_str_destroy(vt_str_t *s);

/** Duplicates and returns a new dynamic string
    @param s vt_str_t instance
    @param alloctr allocator instance

    @returns vt_str_t* instance upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then vt_calloc/realloc/free is used.
*/
extern vt_str_t *vt_str_dup(const vt_str_t *const s, const struct VitaBaseAllocatorType *const alloctr);

/** Takes ownership of an allocated string instead of allocating memory itself
    @param z a raw C string allocated on heap
    @param alloctr allocator instance

    @returns vt_str_t* instance upon success, `NULL` otherwise

    @note if `alloctr = NULL` is specified, then vt_calloc/realloc/free is used.
*/
extern vt_str_t *vt_str_take_ownership(const char *const z, const struct VitaBaseAllocatorType *const alloctr);

/** Returns a zero terminated C string
    @param s vt_str_t string
    @returns zero terminated C string upon success, `NULL` otherwise
*/
extern const char *vt_str_z(const vt_str_t *const s);

/** Returns vt_str_t length
    @param s vt_str_t instance
    @returns vt_str_t length
*/
extern size_t vt_str_len(const vt_str_t *const s);

/** Returns vt_str_t capacity
    @param s vt_str_t instance
    @returns vt_str_t capacity
*/
extern size_t vt_str_capacity(const vt_str_t *const s);

/** Returns available space before new allocation is required
    @param s vt_str_t instance
    @returns free space (capacity - length)
*/
extern size_t vt_str_has_space(const vt_str_t *const s);

/** Checks if string is emtpy ("")
    @param s vt_str_t instance
    @returns `true` if length == 0
*/
extern bool vt_str_is_empty(const vt_str_t *const s);

/** Validates string length
    @param s vt_str_t instance
    @returns valid string length

    @note calculates the correct length, resets if neccessary
*/
extern size_t vt_str_validate_len(vt_str_t *const s);

/** Shrinks vt_str_t capacity to its length
    @param s vt_str_t instance
*/
extern void vt_str_shrink(vt_str_t *const s);

/** Clears the vt_str_t (sets length to 0)
    @param s vt_str_t instance
*/
extern void vt_str_clear(vt_str_t *const s);

/** Reserves memory for vt_str_t
    @param s vt_str_t instance
    @param n how many elements to reserve
*/
extern void vt_str_reserve(vt_str_t *const s, const size_t n);

/** Resizes vt_str_t length
    @param s vt_str_t instance
    @param n resize
*/
extern void vt_str_resize(vt_str_t *const s, const size_t n);

/** Assigns a new raw C string to vt_str_t
    @param s vt_str_t instance
    @param z raw C string

    @returns enum VitaStatus
*/
extern enum VitaStatus vt_str_set(vt_str_t *const s, const char *z);

/** Assigns n characters of raw C string to vt_str_t
    @param s vt_str_t instance
    @param z raw C string
    @param n number of characters

    @returns enum VitaStatus
*/
extern enum VitaStatus vt_str_set_n(vt_str_t *const s, const char *z, const size_t n);

/** Appends a raw C string at the end of vt_str_t
    @param s vt_str_t instance
    @param z raw C string
*/
extern void vt_str_append(vt_str_t *const s, const char *z);

/** Appends a formatted raw C string at the end of vt_str_t
    @param s vt_str_t instance
    @param fmt format

    @returns enum VitaStatus
*/
extern enum VitaStatus vt_str_appendf(vt_str_t *const s, const char *const fmt, ...);

/** Appends n characters of raw C string at the end of vt_str_t
    @param s vt_str_t instance
    @param z raw C string
    @param n number of characters
*/
extern void vt_str_append_n(vt_str_t *const s, const char *z, const size_t n);

/** Inserts a raw C string into vt_str_t starting at the specified index
    @param s vt_str_t instance
    @param z raw C string
    @param at start at index (including `at`)
*/
extern void vt_str_insert(vt_str_t *const s, const char *z, const size_t at);

/** Removes n chars from vt_str_t, starting from the specified index
    @param s vt_str_t instance
    @param from start from index
    @param n number of elements to remove after `from`
*/
extern void vt_str_remove(vt_str_t *const s, const size_t from, size_t n);

/** Removes the first encountered substring from vt_str_t
    @param s vt_str_t instance
    @param z raw C string

    @returns enum VitaStatus
*/
extern enum VitaStatus vt_str_remove_first(vt_str_t *const s, const char *z);

/** Removes the last encountered substring from vt_str_t
    @param s vt_str_t instance
    @param z raw C string

    @returns enum VitaStatus
*/
extern enum VitaStatus vt_str_remove_last(vt_str_t *s, const char *const z);

/** Removes all instances of encountered substring from vt_str_t
    @param s vt_str_t instance
    @param z raw C string

    @returns enum VitaStatus
*/
extern enum VitaStatus vt_str_remove_all(vt_str_t *const s, const char *z);

/** Removes all encountered characters specified by the user from vt_str_t
    @param s vt_str_t instance
    @param c characters to remove one after another: "\\n ," => remove new line, whitespace, comma
*/
extern void vt_str_remove_c(vt_str_t *const s, const char *const c);

/** Strips leading and tailing whitespace and control symbols
    @param s vt_str_t instance
*/
extern void vt_str_strip(vt_str_t *const s);

/** Strips leading and tailing punctuation marks + whitespace and control symbols
    @param s vt_str_t instance
*/
extern void vt_str_strip_punct(vt_str_t *const s);

/** Strips leading and tailing characters specified by the user
    @param s vt_str_t instance
    @param c characters to strip one after another: "\n ," => strip new line, whitespace, comma
*/
extern void vt_str_strip_c(vt_str_t *const s, const char *const c);

/** Find a substring
    @param s vt_str_t instance
    @param z raw C string

    @returns pointer to the begining of a substring in a string, or `NULL` upon failure
*/
extern const char *vt_str_find(const char *const z, const char *sub);

/** Checks if vt_str_t contains a substring
    @param s vt_str_t instance
    @param z raw C string

    @returns number of substring instances in `vt_str_t`
*/
extern size_t vt_str_can_find(const vt_str_t *const s, const char *z);

/** Splits a string given a separator into substrings
    @param p vt_plist_t instance, if `NULL` allocates
    @param s vt_str_t instance
    @param sep seperator string

    @returns `vt_plist_t` of `vt_str_t`, `NULL` upon failure
*/
extern vt_plist_t *vt_str_split(vt_plist_t *ps, const vt_str_t *const s, const char *const sep);

/** Splits a string between two substrings
    @param s vt_str_t instance where the result will be saved, if `NULL` allocates
    @param z raw C string
    @param zl left substring
    @param zr right substring

    @returns `vt_str_t` upon success, `NULL` upon failure
*/
extern vt_str_t *vt_str_split_between(vt_str_t *const s, const char *const z, const char *const zl, const char *const zr);

/** Joins strings by separator
    @param s vt_str_t instance where the result will be saved, if `NULL` allocates
    @param sep C string separator that will be used to join strings together
    @param p list of strings

    @returns `vt_str_t` joined string, `NULL` upon failure
*/
extern vt_str_t *vt_str_join(vt_str_t *const s, const char *const sep, const vt_plist_t *const p);

/** Joins strings by separator
    @param s vt_str_t instance where the result will be saved, if `NULL` allocates
    @param sep C string separator that will be used to join strings together
    @param n number of strings to join
    @param ... strings to join

    @returns `vt_str_t` joined string, `NULL` upon failure
*/
extern vt_str_t *vt_str_join_n(vt_str_t *const s, const char *const sep, const size_t n, ...);

/** Pops off the first part of the string before the separator
    @param sr vt_str_t instance where the result will be saved, if NULL is passed, it's allocated
    @param s vt_str_t instance
    @param sep seperator string

    @returns vt_str_t, `NULL` upon failure
*/
extern vt_str_t *vt_str_pop_get_first(vt_str_t *sr, vt_str_t *const s, const char *const sep);

/** Pops off the last part of the string after the separator
    @param sr vt_str_t instance where the result will be saved, if NULL is passed, it's allocated
    @param s vt_str_t instance
    @param sep seperator string

    @returns vt_str_t, `NULL` upon failure
*/
extern vt_str_t *vt_str_pop_get_last(vt_str_t *sr, vt_str_t *const s, const char *const sep);

/** Checks if two raw C strings are the same
    @param z1 raw C string
    @param z2 raw C string

    @returns `true` if z1 == z2
*/
extern bool vt_str_equals(const char *const z1, const char *const z2);

/** Checks if a raw C string starts with a substring
    @param z raw C string
    @param sub raw C substring

    @returns `true` if z starts with sub
*/
extern bool vt_str_starts_with(const char *const z, const char *const sub);

/** Checks if a raw C string ends with a substring
    @param z raw C string
    @param sub raw C substring

    @returns `true` if z ends with sub
*/
extern bool vt_str_ends_with(const char *const z, const char *const sub);

/** Applies a user specified function upon each char
    @param s vt_str_t
    @param func function to execute upon each element: func(char pointer, for loop index) 
*/
extern void vt_str_apply(const vt_str_t *const s, void (*func)(char*, size_t));

/** Checks if the entire string is a number
    @param z raw C string
    @param max_len max len to top checking (internally it uses strnlen)

    @returns true upon z being a number
*/
extern bool vt_str_is_numeric(const char *const z, const size_t max_len);

/** Capitalizes a string
    @param s vt_str_t
*/
extern void vt_str_capitalize(vt_str_t *const s);

/** Returns the first occurance of char in string
    @param s vt_str_t
    @param z character 

    @returns position of z in vt_str_t upon success, -1 upon failure
*/
extern int64_t vt_str_index_of(const vt_str_t *const s, const char z);

/** Slides through the container elements one by one
    @param s vt_str_t pointer
    @returns container ptr head pointing to next element from the start

    @note returns `NULL` upon reaching the end
*/
extern char *vt_str_slide_front(vt_str_t *const s);

/** Slides through the container elements one by one
    @param s vt_str_t pointer
    @returns container ptr head pointing to next element from the end

    @note returns `NULL` upon reaching the end
*/
extern char *vt_str_slide_back(vt_str_t *const s);

/** Resets the slider
    @param s vt_str_t pointer
*/
extern void vt_str_slide_reset(vt_str_t *const s);

#endif // VITA_STR_H
