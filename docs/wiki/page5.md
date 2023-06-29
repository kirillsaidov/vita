# Parsing command line arguments
In this chapter we discuss how to parse command line arguments with `Vita`. 

## Contents
* [Declaring our variables and command options](page5.md#declaring-our-variables-and-command-options)
* [Parsing command line arguments](page5.md#parsing-command-line-arguments-1)
* [Print help manual](page5.md#print-help-manual)

### Declaring our variables and command options
First, we need to declare our variables:
```c
#include "vita/util/argopt.h"

int32_t main(const int32_t argc, const char *argv[]) {
    // declare variables
    const char *opt_youtube_link    = NULL;
    int32_t     opt_video_quality   = 720;
    bool        opt_verbose         = false;

    // ...
```

Secondly, we need to create an `vt_argopt_t` array instance and describe our variables so it knows what and how to parse:
```c
    vt_argopt_t optv[] = {
        // long      short   description            argument                   type
        { "--link",    "-l", "youtube link",   VT_ARGOPT(opt_youtube_link),  VT_TYPE_CSTR  },
        { "--quality", "-q", "video quality",  VT_ARGOPT(opt_video_quality), VT_TYPE_INT32 },
        { "--verbose", "-v", "verbose output", VT_ARGOPT(opt_verbose),       VT_TYPE_BOOL  },
    };
    const size_t optc = sizeof(optv)/sizeof(vt_argopt_t);
```

### Parsing command line arguments
The next step is to parse command line options and check the return value returned by `vt_argopt_parse`:
```c
    // parse args and opts
    const int8_t parse_status = vt_argopt_parse(argc, argv, optc, optv);
    if(parse_status < 0) { // or (parse_status == ARGOPT_PARSE_ERROR)
        printf("See 'argopt -h' for more info!\n");
        goto cleanup;
    }
```

### Print help manual
Finally, we can print the help manual in case it is needed:
```c
    // display help manual
    if(parse_status == ARGOPT_PARSE_HELP_WANTED) {
        vt_argopt_print_help(
            "argopt v0.3.0 -- Testing argopt parser",                   // header
            "Example: argopt --link my_youtube_link -q 1080 --verbose", // footer
            optc, optv
        );
        return 0;
    }
```

Now you can do your thing:
```c
    const bool success = download_youtube_video(opt_youtube_link, opt_video_quality, opt_verbose);
    if(!success) {
        // error handling
        // ...
    }
```

And do not forget to free allocated resorces:
```c
    // ...

cleanup:
    free(opt_youtube_link); // argopt uses dynamic strings

    return 0;
} // main
```

Read more at [util/argopt.h](../../inc/vita/util/argopt.h) or [test_argopt.c](../../tests/src/test_argopt.c).

**[ [Back](page4.md) | [Next](page6.md) ]**
