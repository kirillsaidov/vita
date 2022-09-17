#include <assert.h>

#include "../../inc/vita/util/log.h"

int32_t main(void) {
    const char *const logger_filename = "other/test_logger.log";

    assert(str_equals(log_get_level_str(ll_fatal), "FATAL"));
    assert(str_equals(log_get_level_str(-1), "INFO") && str_equals(log_get_level_str(ll_count), "INFO"));
    
    // --- outputs to stderr
    LOG_INFO("[s] This is an info test. 12345 qwerty.");
    LOG_WARN("[s] Testing %s formatter.", "LOG");
    LOG_DEBUG("[s] Testing: debugging = %s, %s", "debug msg", "debug2");
    LOG_ERROR("[s] Error: %s", "testing...");

    // -- outputs to 'logger' file
    // FIXME: fails to log to a file after setting all lob_levels to that file
    log_set_level_default(logger_filename);
    LOG_INFO("[f] This is an info test.");
    LOG_WARN("[f] Testing %s formatter.", "LOG");
    LOG_DEBUG("[f] Testing: debugging = %s, %s", "debug msg", "debug2");
    LOG_ERROR("[f] Error: %s", "testing...");

    //LOG_FATAL("Must crash after logging this message."); // exits after logging the message
    
    // --- set custom file to each log level
    // log_set_level(ll_info, "src/test_log_info.log");
    // log_set_level(ll_error, "src/test_log_error.log");

    // --- logs to files
    // int32_t a = 100;
    // LOG_INFO("This is an info test.");
    // LOG_ERROR("Error: %s", "testing...");
    // LOG_INFO("Logging to %s", "file");
    // LOG_ERROR("Error %s to file (%d)", "logging", a);

    // --- prints to 'logger' file using custom file logger
    LOGF_INFO(logger_filename, "This is a test. %s", "Trying out new functionality.");
    LOGF_WARN(logger_filename, "This is a test. %s", "Trying out new functionality.");
    LOGF_ERROR(logger_filename, "This is a test. %s", "Trying out new functionality.");

    // log assert
    LOG_ASSERT(1, "%s\n", "Hello, world!"); // does not print to stderr
    // LOG_ASSERT(0, "%s\n", "Hello, world!"); // prints to stderr

    return 0;
}


