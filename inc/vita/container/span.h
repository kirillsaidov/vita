#ifndef VITA_CONTAINER_SPAN_H
#define VITA_CONTAINER_SPAN_H

/** SPAN MODULE (view)
    - vt_span_from
    - vt_span_from_to
    - vt_span_len
    - vt_span_get
    - vt_span_getT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
    - vt_span_set
    - vt_span_setT (T = i8, u8, i16, u16, i32, u32, i64, u64, f, d, r)
*/

#include "common.h"
#include "vec.h"
#include "str.h"
#include "plist.h"

// A reference type of contiguous sequence of values of type T. 
// It never allocates, nor deallocates anything and does not keep smart pointers alive.
typedef struct {
    struct VitaBaseArrayType instance;
} vt_span_t;

/** Creates a span from raw data of T*
    @param ptr data
    @param length ditto
    @param elsize element size
    @returns vt_span_t
*/
extern vt_span_t vt_span_from(void *ptr, const size_t length, const size_t elsize);

/** Creates a span from raw data of T* in range [from_idx; to_idx)
    @param ptr data
    @param from_idx from index
    @param to_idx to index
    @param elsize element size
    @returns vt_span_t
*/
extern vt_span_t vt_span_from_to(void *ptr, const size_t from_idx, const size_t to_idx, const size_t elsize);

/** Returns vt_span_t length
    @param span vt_span_t instance
    @returns size_t
*/
extern size_t vt_span_len(const vt_span_t span);

/** Returns value at index
    @param span vt_span_t instance
    @param at index
    @returns void*
*/
extern void *vt_span_get(const vt_span_t span, const size_t at);

/** Returns value at index
    @param span vt_span_t instance
    @param at index
    @returns value
*/
#define VT_PROTOTYPE_SPAN_GET(T, t) extern T vt_span_get##t(const vt_span_t span, const size_t at)
VT_PROTOTYPE_SPAN_GET(int8_t, i8);
VT_PROTOTYPE_SPAN_GET(uint8_t, u8);
VT_PROTOTYPE_SPAN_GET(int16_t, i16);
VT_PROTOTYPE_SPAN_GET(uint16_t, u16);
VT_PROTOTYPE_SPAN_GET(int32_t, i32);
VT_PROTOTYPE_SPAN_GET(uint32_t, u32);
VT_PROTOTYPE_SPAN_GET(int64_t, i64);
VT_PROTOTYPE_SPAN_GET(uint64_t, u64);
VT_PROTOTYPE_SPAN_GET(float, f);
VT_PROTOTYPE_SPAN_GET(double, d);
VT_PROTOTYPE_SPAN_GET(real, r);
#undef VT_PROTOTYPE_SPAN_GET

/** Assigns a new value at an index
    @param span vt_span_t instance
    @param val value
    @param at index to set the value
*/
extern void vt_span_set(vt_span_t span, const void *const val, const size_t at);

/** Assigns a new value at an index
    @param span vt_span_t instance
    @param val value
    @param at index to set the value
*/
#define VT_PROTOTYPE_SPAN_SET(T, t) extern void vt_span_set##t(vt_span_t span, const T val, const size_t at)
VT_PROTOTYPE_SPAN_SET(int8_t, i8);
VT_PROTOTYPE_SPAN_SET(uint8_t, u8);
VT_PROTOTYPE_SPAN_SET(int16_t, i16);
VT_PROTOTYPE_SPAN_SET(uint16_t, u16);
VT_PROTOTYPE_SPAN_SET(int32_t, i32);
VT_PROTOTYPE_SPAN_SET(uint32_t, u32);
VT_PROTOTYPE_SPAN_SET(int64_t, i64);
VT_PROTOTYPE_SPAN_SET(uint64_t, u64);
VT_PROTOTYPE_SPAN_SET(float, f);
VT_PROTOTYPE_SPAN_SET(double, d);
VT_PROTOTYPE_SPAN_SET(real, r);
#undef VT_PROTOTYPE_SPAN_SET

#endif // VITA_CONTAINER_SPAN_H

