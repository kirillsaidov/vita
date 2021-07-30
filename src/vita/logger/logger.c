#include "vita/logger/logger.h"

void logger_info(const str_t msg) {
    printf("INFO [-]: %s\n", as_cstr(msg));
}

void logger_warn(const str_t msg, const str_t topic) {
    printf("WARNING [%s]: %s\n", as_cstr(topic), as_cstr(msg));
}

void logger_error(const str_t msg, const str_t topic) {
    fprintf(stderr, "ERROR [%s]: %s\n", as_cstr(topic), as_cstr(msg));
}

void logger_fatal(const str_t msg, const str_t topic) {
    fprintf(stderr, "FATAL [%s]: %s\n", as_cstr(topic), as_cstr(msg));
    exit(1);
}














