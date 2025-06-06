<img src="imgs/icon.png" width="64" height="64" align="left"></img>
# Vita

A simple and intuitive standard library for your modern C project! Lightweight and no dependencies. 

`Vita` strives to provide all common functionality other modern libraries support to ease prototyping with C and simplify the development process.

## Features
* Written in C11
* Containers
* String manipulation
* Path and file handling
* Logging facility
* Command line arguments parser
* Facilities to work with paths and directories
* Datetime utility
* Simple sockets wrapper

### Arguments parser showcase
```c
// argopt_showcase.c
#include "vita/vita.h"

int main(const int argc, const char *argv[]) {
    char   *opt_link = NULL;
    char   *opt_save = strdup("~/Desktop");
    bool    opt_verbose = false;
    int32_t opt_quality = 720;

    // create options to parse
    vt_argopt_t optv[] = {
        //  long        short     description          argument              type
        { "--link",     "-l",   "url link",       VT_ARGOPT(opt_link),    VT_TYPE_CSTR },
        { "--save",     "-s",   "save directory", VT_ARGOPT(opt_save),    VT_TYPE_CSTR },
        { "--verbose",  "-v",   "verbose output", VT_ARGOPT(opt_verbose), VT_TYPE_BOOL },
        { "--quality",  "-q",   "video quality",  VT_ARGOPT(opt_quality), VT_TYPE_INT32 },
    };
    const size_t optc = sizeof(optv)/sizeof(vt_argopt_t);

    // parse args and opts
    const int8_t parse_status = vt_argopt_parse(argc, argv, optc, optv, NULL);
    if (parse_status < 0) {
        printf("See 'argopt -h' for more info!\n");
        goto cleanup;
    }

    // display help manual
    if (parse_status == VT_ARGOPT_PARSE_HELP_WANTED) {
        vt_argopt_print_help(
            "downloader v0.3.0 -- easy video downloader",                    // header
            "Example: downloader --link your_url --save save/dir --verbose", // footer
            optc, optv
        );
        return 0;
    }

    // now do your thing
    download(opt_link, opt_save, opt_verbose, opt_quality);

cleanup:
    free(opt_link);
    free(opt_save);

    return 0;
}
```

Folder structure:
```c
📁 project  
├── 📁 vita         // vita includes
├── 📄 main.c       // our program
└── 📄 libvita.a    // pre-built static library
```

Building and running:
```sh
$ gcc main.c -o downloader -lvita -L.
$ ./downloader --link "www.abs.com/video" -s ~/Desktop/media --verbose
==============================================
Downloading video with the following options:
        url: www.abs.com/video
       save: ~/Desktop/media
    verbose: true
    quality: 720
==============================================

$ ./downloader -h
downloader v0.3.0 -- easy video downloader
-l    --link url link
-s    --save save directory
-v --verbose verbose output
-q --quality video quality
-h    --help This help information.
Example: downloader --link your_url --save save/dir --verbose
```

## More examples
Check out the [Vita Wiki](docs/wiki/VITA.md) or take a look at [`tests/src`](tests/src) for more examples.

## Development process
You can also closely follow the development process in Trello [board](https://trello.com/b/MFeDGO8u/vita). If you face any errors, please report by PR or create an issue. 

## LICENSE
All code is licensed under the BSL license.

