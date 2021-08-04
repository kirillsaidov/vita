#ifndef VITA_STRBUF_H
#define VITA_STRBUF_H

/** VITA_STRBUF MODULE (dynamic string)
    - strbuf_manual_collect

    - strbuf_memhandler_internal_create
    - strbuf_memhandler_internal_destroy
    - strbuf_memhandler_internal

    - strbuf 
    - strbuf_new (calls strbuf)
    - strbuf_dup *
    - strbuf_free

    - strbuf_cstr
    - strbuf_len
    - strbuf_capacity
    - strbuf_has_space

    - strbuf_shrink
    - strbuf_reserve
    - strbuf_set *
    - strbuf_append
    - strbuf_insert
    - strbuf_remove
    - strbuf_remove_str
    - strbuf_remove_str_all
    - strbuf_contains
    - strbuf_split *
    - strbuf_pop_first *
    - strbuf_pop_last *
    - strbuf_equals *
*/

#include "str.h"
#include "../memory/memhandler.h"

// strbuf struct
typedef str_t* strbuf_pt;

/** strbuf_manual_collect ==> memory management type: manual or through the strbuf internal memory handler
*/
extern void strbuf_manual_collect(const bool status);







/** strbuf_memhandler_internal_create ==> creates an internal memory handler for strbuf
*/
extern bool strbuf_memhandler_internal_create(void);

/** strbuf_memhandler_internal_destroy ==> destroys the strbuf internal memory handler
*/
extern void strbuf_memhandler_internal_destroy(void);

/** strbuf_memhandler_internal ==> returns the strbuf internal memory handler
*/
extern const memhandler_pt strbuf_memhandler_internal(void);







/** strbuf ==> creates a new dynamic string
    params:
        const str_t s
    returns:
        strbuf_pt
    notes:
        allocates additional memory for the '\0' terminator automatically
        allocates additional 1/3 of the original memory size for appending (use `strbuf_shrink` to remove additional free memory)
*/
extern strbuf_pt strbuf(const str_t s);

/** strbuf ==> creates a new dynamic string (calls strbuf)
    params:
        const str_t s
    returns:
        strbuf_pt
*/
extern strbuf_pt strbuf_new(const str_t s);

/** strbuf ==> frees a dynamic string
    params:
        strbuf_pt sb
*/
extern void strbuf_free(strbuf_pt sb);







/** strbuf_cstr ==> returns the char* pointer to string buffer
    params:
        const strbuf_pt sb
    returns:
        const char*
*/
extern const char* strbuf_cstr(const strbuf_pt sb);

/** strbuf_len ==> returns string length
    params:
        const strbuf_pt sb
    returns:
        size_t (str length)
    notes:
        the '\0' terminator is excluded
*/
extern size_t strbuf_len(const strbuf_pt sb);

/** strbuf_capacity ==> returns string capacity
    params:
        const strbuf_pt sb
    returns:
        size_t (str capacity)
*/
extern size_t strbuf_capacity(const strbuf_pt sb);

/** strbuf_has_space ==> returns available space before a new allocation will be required
    params:
        const strbuf_pt sb
    returns:
        size_t (capacity - length)
*/
extern size_t strbuf_has_space(const strbuf_pt sb);







/** strbuf_shrink ==> shrinks string's capacity to its length
    params:
        strbuf_pt sb
    returns:
        bool
    notes:
        true    upon sucess
        false   upon failure
*/
extern bool strbuf_shrink(strbuf_pt sb);

/** strbuf_reserve ==> reserves memory for string
    params:
        strbuf_pt sb
        const size_t n
    returns:
        bool
    notes:
        true    upon sucess
        false   upon failure
*/
extern bool strbuf_reserve(strbuf_pt sb, const size_t n);

/** strbuf_append ==> appends at the end of the string
    params:
        strbuf_pt sb
        const str_t s
    returns:
        bool
    notes:
        true    upon sucess
        false   upon failure
*/
extern bool strbuf_append(strbuf_pt sb, const str_t s);

/** strbuf_insert ==> inserts a string into strbuf starting the specified index
    params:
        strbuf_pt sb
        const str_t s
        const size_t from
    returns:
        bool
    notes:
        true    upon sucess
        false   upon failure
*/
extern bool strbuf_insert(strbuf_pt sb, const str_t s, const size_t fromIndex);

/** strbuf_remove ==> removes a n chars from strbuf starting from the specified index
    params:
        strbuf_pt sb
        const size_t from
        const size_t n
    returns:
        bool
    notes:
        true    upon sucess
        false   upon failure
*/
extern bool strbuf_remove(strbuf_pt sb, const size_t fromIndex, const size_t n);

/** strbuf_remove_str ==> removes the first encountered substring from strbuf, if not found, returns from function
    params:
        strbuf_pt sb
        const str_t s
    returns:
        bool
    notes:
        true    upon sucess
        false   upon failure
*/
extern bool strbuf_remove_str(strbuf_pt sb, const str_t s);

/** strbuf_remove_str ==> removes all instances of a substring from strbuf (calls strbuf_remove_str)
    params:
        strbuf_pt sb
        const str_t s
    returns:
        bool
    notes:
        true    upon sucess
        false   upon failure
*/
extern bool strbuf_remove_str_all(strbuf_pt sb, const str_t s);

/** strbuf_contains ==> check is substring is contained in strbuf
    params:
        const strbuf_pt sb
        const str_t s
    returns:
        size_t (number of substring instances in strbuf)
*/
extern size_t strbuf_contains(const strbuf_pt sb, const str_t s);

/** strbuf_split ==> check is substring is contained in strbuf
    params:
        const strbuf_pt sb
        const str_t s
    returns:
        strbuf_pt* strbufList
    notes:
        valid ptr	if substrings were found in strbuf
        NULL		upon failure
*/
// extern size_t strbuf_split(const strbuf_pt sb, const str_t s, strbuf_pt* sblist);

#endif // VITA_STRBUF_H



















