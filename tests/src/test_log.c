#include <assert.h>

#include "../../inc/vita/util/log.h"

int32_t main(void) {
    const char *const logger_filename = "other/test_logger.log";

    assert(vt_str_equals(vt_log_get_level_str(vt_log_fatal), "FATAL"));
    // vt_log_get_level_str(vt_log_count) // fails constraint [ arg < vt_log_count ] 

    // --- outputs to stderr
    VT_LOG_INFO("[s] This is an info test. 12345 qwerty.");
    VT_LOG_WARN("[s] Testing %s formatter.", "LOG");
    VT_LOG_DEBUG("[s] Testing: debugging = %s, %s", "debug msg", "debug2");
    VT_LOG_ERROR("[s] Error: %s", "testing...");

    // -- outputs to 'logger' file
    // FIXME: fails to log to a file after setting all lob_levels to that file
    vt_log_redirect_all_output(logger_filename);
    VT_LOG_INFO("[f] This is an info test.");
    VT_LOG_WARN("[f] Testing %s formatter.", "LOG");
    VT_LOG_DEBUG("[f] Testing: debugging = %s, %s", "debug msg", "debug2");
    VT_LOG_ERROR("[f] Error: %s", "testing...");

    //VT_LOG_FATAL("Must crash after logging this message."); // exits after logging the message
    
    // --- set custom file to each log level
    // vt_log_redirect_level_output(ll_info, "src/test_vt_log_info.log");
    // vt_log_redirect_level_output(ll_error, "src/test_vt_log_error.log");

    // --- logs to files
    // int32_t a = 100;
    // VT_LOG_INFO("This is an info test.");
    // VT_LOG_ERROR("Error: %s", "testing...");
    // VT_LOG_INFO("Logging to %s", "file");
    // VT_LOG_ERROR("Error %s to file (%d)", "logging", a);

    // --- prints to 'logger' file using custom file logger
    VT_LOGF_INFO(logger_filename, "This is a test. %s", "Trying out new functionality.");
    VT_LOGF_WARN(logger_filename, "This is a test. %s", "Trying out new functionality.");
    VT_LOGF_ERROR(logger_filename, "This is a test. %s", "Trying out new functionality.");

    // log assert
    VT_LOG_ASSERT(1, "%s\n", "Hello, world!"); // does not print to stderr
    // VT_LOG_ASSERT(0, "%s\n", "Hello, world!"); // prints to stderr

    return 0;
}


