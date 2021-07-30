#include "vita/logger.h"

void logger_info(const char* msg, const char* topic) {
	if(topic == NULL) {
		fprintf(stderr, "INFO: %s\n", msg);
	} else {
		fprintf(stderr, "INFO [%s]: %s\n", topic, msg);
	}
}

void logger(const char* type, const char* msg, const char* topic) {
	if(topic == NULL) {
		fprintf(stderr, "%s: %s\n", type, msg);
	} else {
		fprintf(stderr, "%s [%s]: %s\n", type, topic, msg);
	}
}















