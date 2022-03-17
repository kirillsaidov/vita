#include <assert.h>

#include "../../inc/vita/util/log.h"

int main(void) {
    assert(log_get_level_string(ll_fatal) == "FATAL");
    assert(log_get_level_string(-1) == "INFO" && log_get_level_string(ll_count) == "INFO");
    
    //LOG_INFO("This is an info test.");
    //LOG_WARN("Testing %s formatter.", "LOG");
    //LOG_DEBUG("Testing: debugging = %s, %s", "debug msg", "debug2");
    //LOG_ERROR("Error: %s", "testing...");
    //LOG_FATAL("Must crash after logging this message."); // exits after logging the message
    
    // log level
    log_set_level(ll_info, "src/test_log_info.log.txt");
    log_set_level(ll_error, "src/test_log_error.log.txt");

    // logs to files
    int a = 100;
    LOG_INFO("This is an info test.");
    LOG_ERROR("Error: %s", "testing...");
    LOG_INFO("Logging to %s", "file");
    LOG_ERROR("Error %s to file (%d)", "logging", a);

    return 0;
}


