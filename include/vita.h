#ifndef VITA_H
#define VITA_H

/** VITA MODULE
	- vita_init
	- vita_flush
*/

// vita includes
#include "vita/core/core.h"
#include "vita/memory/mem.h"
#include "vita/memory/memhandler.h"
#include "vita/string/str.h"
#include "vita/logger/logger.h"

// Vita init enum
enum EVitaInit {
	EVitaInit_MEMHANDLER_ID,
	EVitaInit_SUCCESS
};

/** vita_flush ==> initializes and allocates memory for all vita internal memory handlers
*/
extern enum EVitaInit vita_init(void);

/** vita_flush ==> frees all memory allocated by internal memory handlers
*/
extern void vita_flush(void);

#endif // VITA_H

















