#include <assert.h>

#include "../../inc/vita/util/log.h"

int32_t main(void) {
    assert(str_equals(log_get_level_str(ll_fatal), "FATAL"));
    assert(str_equals(log_get_level_str(-1), "INFO") && str_equals(log_get_level_str(ll_count), "INFO"));
    
    //LOG_INFO("This is an info test.");
    //LOG_WARN("Testing %s formatter.", "LOG");
    //LOG_DEBUG("Testing: debugging = %s, %s", "debug msg", "debug2");
    //LOG_ERROR("Error: %s", "testing...");
    //LOG_FATAL("Must crash after logging this message."); // exits after logging the message
    
    // log level
    // log_set_level(ll_info, "src/test_log_info.log.txt");
    // log_set_level(ll_error, "src/test_log_error.log.txt");

    // logs to files
    // int32_t a = 100;
    // LOG_INFO("This is an info test.");
    // LOG_ERROR("Error: %s", "testing...");
    // LOG_INFO("Logging to %s", "file");
    // LOG_ERROR("Error %s to file (%d)", "logging", a);

    // log assert
    LOG_ASSERT(1, "%s\n", "Hello, world!"); // does not print to stderr
    // LOG_ASSERT(0, "%s\n", "Hello, world!"); // prints to stderr

    return 0;
}


