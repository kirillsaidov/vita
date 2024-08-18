#ifndef VITA_CONTAINER_SLLIST_H
#define VITA_CONTAINER_SLLIST_H

/** SINGLE LINKED LIST MODULE
    - vt_sllist_create
    - vt_sllist_destroy
    - vt_sllist_len
    - vt_sllist_is_empty
    - vt_sllist_clear
    - vt_sllist_set
    - vt_sllist_get
    - vt_sllist_push_front
    - vt_sllist_push_back
    - vt_sllist_pop_front
    - vt_sllist_pop_back
    - vt_sllist_pop_get_front
    - vt_sllist_pop_get_back
    - vt_sllist_remove
    - vt_sllist_remove_element
    - vt_sllist_apply
*/

#include "vita/container/common.h"

/// single linked list structure
typedef struct VitaSingleLinkedList {
    size_t len;
    struct VitaNode *head;
} vt_sllist_t;



#endif // VITA_CONTAINER_SLLIST_H

