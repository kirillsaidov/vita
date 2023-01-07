<img src="imgs/v-flaticon.png" width="64" height="64" align="left"></img>
# Vita

A custom C library for quick prototyping with simple data structures, string manipulation, file and path handling, command line arguments parser and more!

## Features
* simple containers (vec, mat, plist)
* string manipulation
* path and file handling
* logging facility
* command line arguments parsing
* facilities to work with paths and directories

### Arguments parser showcase
```c
// argopt_showcase.c
#include "vita/vita.h"

int32_t main(const int32_t argc, const char *argv[]) {
    char *app_savedir = strdup("./media");
    int32_t app_max_grab_files = 10;
    bool app_verbose = false;

    // create options to parse
    argopt_t optv[] = {
          // long       short     description           argument               type
        { "--verbose",  "-v", "verbose output",   ARGOPT(app_verbose),        dt_bool },
        { "--max",      "-m", "max grab files",   ARGOPT(app_max_grab_files), dt_int32 },
        { "--save",     "-s", "save directory",   ARGOPT(app_savedir),        dt_cstr },
    };
    const size_t optc = sizeof(optv)/sizeof(argopt_t);

    // parse args and opts
    const int8_t parse_status = argopt_parse(argc, argv, optc, optv);
    if(parse_status < 0) { // or (parse_status == ARGOPT_PARSE_ERROR)
        printf("See 'argopt -h' for more info!\n");
        goto cleanup;
    }

    // display help manual
    if(parse_status == ARGOPT_PARSE_HELP_WANTED) {
        argopt_print_help(
            "argopt v0.3.0 -- Testing argopt parser",               // header
            "Example: argopt --max 15 -s my/save/dir/ --verbose",   // footer
            optc, optv
        );
        return 0;
    }

    // now do your thing
    printf("%20s : %d\n", "app_verbose", app_verbose);
    printf("%20s : %d\n", "app_max_grab_files", app_max_grab_files);
    printf("%20s : %s\n", "app_savedir", app_savedir);

cleanup:
    free(app_savedir);

    return 0;
}
```

Building and running:
```
$ gcc argopt_showcase.c -o argopt -lvita -L.
$ ./argopt --max 15 -s my/save/dir/ --verbose
        app_verbose : 1
 app_max_grab_files : 15
        app_savedir : my/save/dir/

$ ./argopt -h
argopt v0.3.0 -- Testing argopt parser
-v --verbose verbose output
-m     --max max grab files
-s    --save save directory
-h    --help This help information.
Example: argopt --max 15 -s my/save/dir/ --verbose
```

## More examples
Check out the [`tests/src`](tests/src) directory for usage examples or read more on [Vita Wiki](docs/wiki/VITA.md).

#### Dev-process
You can also closely follow the development process in Trello [board](https://trello.com/b/MFeDGO8u/vita).

## LICENSE
All code is licensed under the BSL license.
