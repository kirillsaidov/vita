#include "vita/container/plist.h"


plist_t *plist_new(void);
enum ContainerError plist_ctor(plist_t *p, const size_t n);
enum ContainerError plist_dtor(plist_t *p);
void plist_free(plist_t *p);







plist_t *plist_create(const size_t n);
void plist_destroy(plist_t *p);







size_t plist_len(const plist_t *const p);
size_t plist_capacity(const plist_t *const p);
size_t plist_has_space(const plist_t *const p);







enum ContainerError plist_reserve(plist_t *const p, const size_t n);
enum ContainerError plist_shrink(plist_t *const p);
enum ContainerError plist_set(plist_t *const p, const void *ptr, const size_t at);
void *plist_get(const plist_t *const p, const size_t at);
enum ContainerError plist_push(plist_t *const p, const void *ptr);
enum ContainerError plist_pop(plist_t *const p);
enum ContainerError plist_remove(plist_t *const p, const size_t at);
void plist_foreach(const plist_t *const p, void (*func)(void*, size_t, size_t));















