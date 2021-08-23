#include "vita/container/plist.h"

plist_pt plist() {
	return plist_new(7);
}

plist_pt plist_new(const size_t n) {
	// create a plist instance
	plist_pt plist = ((array_manual_collect_status()) ? 
		(mem_malloc(1, sizeof(struct BaseArrayType))) : 
		(memhandler_malloc(array_memhandler_internal(), 1, sizeof(struct BaseArrayType)))
	);

	// checking if plist was allocated
	if(is_null(plist)) {
		logger_error(str("unable to create a plist instance!"), str("plist_new"));

		if(!array_manual_collect_status()) {
			logger_info(str("ManualCollect is false? Then don't forget to initialize the module\'s internal memhandler."));
		}

		return NULL;
	}

	// allocate memory for plist elements
	*plist = (struct BaseArrayType) {
		.p2p = ((array_manual_collect_status()) ? 
			(mem_calloc((n + n/3), sizeof(void*))) : 
			(memhandler_calloc(array_memhandler_internal(), (n + n/3), sizeof(void*)))),
		.len = 0,
		.capacity = (n + n/3)
	};

	// check if plist elements were allocated
	if(is_null(plist->p2p)) {
		logger_error(str("unable to allocate memory for plist elements!"), str("plist_new"));
		return NULL;
	}

	return plist;
}

void plist_free(plist_pt plist) {
	if(is_null(plist)) {
		logger_warn(str("plist is NULL, safely exiting..."), str("plist_free"));
		return;
	}

	// free plist elements and plist instance
	if(array_manual_collect_status()) {
		mem_free(plist->p2p);
		mem_free(plist);
	} else {
		memhandler_free(array_memhandler_internal(), plist->p2p);
		memhandler_free(array_memhandler_internal(), plist);
	}
}

size_t array_len(const plist_pt plist) {
	if(is_null(plist)) {
		return 0;
	} else {
		return plist->len;
	}
}

size_t array_capacity(const plist_pt plist) {
	if(is_null(plist)) {
		return 0;
	} else {
		return plist->capacity;
	}
}

size_t array_has_space(const plist_pt plist) {
	if(is_null(plist)) {
		return 0;
	} else {
		return (plist->capacity - plist->len);
	}
}



















































