# Debugging
`Vita` provides good coverage for debugging your code and enforcing conditions. This code is then removed in release builds. Refer to [`util/debug.h`](../../inc/vita/util/debug.h) for more information.

## Contents
1. [Assert and debug information](page8.md#assert-and-debug-information)
2. [Enforcing conditions even in release builds](page8.md#enforcing-conditions-even-in-release-builds)
3. [Redirect or disable debug output](page8.md#redirect-or-disable-debug-output)

### Assert and debug information
If the specified condition fails, `Vita` assertions exit the program outputting an error message: 
```C
#include "vita/util/debug.h"

void download_file(const char *const url) {
    VT_DEBUG_ASSERT(url != NULL, "No url provided! Error code: %d", download_error_code);
    // output: 2023-09-09 21:13:32 DEBUG ASSERTION FAILURE [url != NULL] download.c:main:4: No url provided! Error code: -1
    
    // do your thing
    // ...
}
```

In case you need only to output the debug information, then you can use `VT_DEBUG_PRINTF`:
```C
Result parse_html(const char *const html) {
    if (!validate_html(html)) {
        VT_DEBUG_PRINTF("Invalid html source! Attempting to fix...\n"); // acts like printf
        fix_html(html);
    }

    // do your thing
    // ...

    return new_result(html);
}
```

All funtionality that has `debug` in its name is removed in release builds! So `VT_DEBUG_ASSERT` and `VT_DEBUG_PRINTF` are removed in release builds as well! 

### Enforcing conditions even in release builds
You can also enforce a condition in release builds using `VT_ENFORCE`. Code below is taken from `Vita` [datetime](../../inc/vita/time/datetime.h) module:
```C
struct VitaDateTime vt_datetime_from_text(const char *timebuf) {
    // removed in relase builds
    VT_DEBUG_ASSERT(timebuf != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    
    // kept in release builds
    VT_ENFORCE(
        vt_datetime_is_valid_text(timebuf), 
        "Invalid timestamp format! Expected \'%s\', but recieved \'%s\'.\n", 
        "YYYY-MM-DD HH:MM:SS", 
        timebuf
    );

    // do your thing
    // ...
}
```

You can also add runtime checks that only output a message if condition fails. It does not interrupt the program flow or exit:
```C
VT_CHECK(
    1 == 2, 
    "If you read this, then the condition has failed! But your program continues to run | read more here %s\n", 
    source
); // printf style formatting
```

### Redirect or disable debug output
```C
// redirect output to a file
vt_debug_redirect_output("file.debug");

// completely disable all debug output
vt_debug_disable_output(true);
```

For more usage examples, read [test_debug.c](../../tests/src/test_debug.c).

**[ [Back](page7.md) | [Next](page9.md) ]**
