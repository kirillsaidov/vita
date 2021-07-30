#ifndef VITA_LOGGER_H
#define VITA_LOGGER_H

/* VITA_LOGGER MODULE
	- logger_info
	- logger
*/

// std includes
#include <stdio.h>

/* logger_info ==> prints msg and topic to stderr
	params:
		const char* msg
		const char* topic
*/
extern void logger_info(const char* msg, const char* topic);

/* logger ==> prints msg and topic and type of msg to stderr
	params:
		const char* type
		const char* msg
		const char* topic
*/
extern void logger(const char* type, const char* msg, const char* topic);

#endif // VITA_LOGGER_H

















