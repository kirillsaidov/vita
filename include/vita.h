#ifndef VITA_H
#define VITA_H

/** VITA MODULE
	- vita_init
	- vita_flush
*/

#include "vita/core/core.h"
#include "vita/core/as.h"

#include "vita/path/path.h"

#include "vita/logger/logger.h"
#include "vita/logger/flogger.h"

#include "vita/memory/mem.h"
#include "vita/memory/memhandler.h"

#include "vita/string/str.h"
#include "vita/string/strbuf.h"

#include "vita/algorithm/search.h"

#include "vita/container/array.h"

// Vita init enum
enum EVitaInit {
	EVitaInit_MEMHANDLER_ID,
	EVitaInit_STRBUF_ID,
	EVitaInit_ARRAY_ID,
	EVitaInit_SUCCESS
};

/** vita_flush ==> initializes and allocates memory for all vita internal memory handlers
*/
extern enum EVitaInit vita_init(void);

/** vita_flush ==> frees all memory allocated by internal memory handlers
*/
extern void vita_flush(void);

#endif // VITA_H

















