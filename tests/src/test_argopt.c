#include <assert.h>

#include "../../inc/vita/util/argopt.h"

int32_t main(void) {
    DEBUG_DEFAULT_INIT();

    // strings
    str_t *rpath = str("my/temp/folder/default/initialized");
    str_t *wpath = NULL;

    // floats
    float intensity = 0.1;
    float polarity = 0.2;

    // bools
    bool verbose = false;
    bool audio = false;

    // ints
    int32_t volume = 50;
    int32_t level = 1;

    // chars
    char update = 'n';
    char upgrade = 'n';

    // arguments
    const char *argv[] = {
        /*  progname: */"./test_argopt",                    // 1

        /*     rpath: */ "-r=../temp",                      // 2
        /*     wpath: */ "--wpath", "./docs/dw/data/",      // 4
        
        /* intensity: */ "--intensity=0.7",                 // 5
        /*  polarity: */ "-p", "0.35",                      // 7
       
        /*   verbose: */ "--verbose",                       // 8
        /*     audio: */ "-a", "true",                      // 10
        
        /*    volume: */ "-vl=83",                          // 11
        /*     level: */ "--level", "13",                   // 13
        
        /*    update: */ "--update=y",                      // 14
        /*   upgrade: */ "-ug", "y",                        // 16
        
        /*   unknown: */ "-uo",                             // 17
        /*   unknown: */ "--unkownOption", "unkownValue",   // 19

        // "-r", "../temp", "--wpath", "./docs/dw/data", "--intensity", "0.7"
    };
    const size_t argc = 19;
    // const size_t argc = 7;

    // options
    argopt_t optv[] = {
        { "--rpath", "-r", "read path", ARGOPT(rpath), dt_str },
        { "--wpath", "-w", "save path", ARGOPT(wpath), dt_str },

        { "--intensity", "-i", "level of intensity between [0; 1]", ARGOPT(intensity), dt_float },
        { "--polarity", "-p", "level of polarity between [0; 1]", ARGOPT(polarity), dt_float },

        { "--verbose", "-v", "verbose output", ARGOPT(verbose), dt_bool },
        { "--audio", "-a", "include audio", ARGOPT(audio), dt_bool },

        { "--volume", "-vl", "volume level", ARGOPT(volume), dt_int32 },
        { "--level", "-l", "output level", ARGOPT(level), dt_int32 },

        { "--update", "-ud", "update (y, n)", ARGOPT(update), dt_char },
        { "--upgrade", "-ug", "upgrade (y, n)", ARGOPT(upgrade), dt_char }
    };
    const size_t optc = sizeof(optv)/sizeof(argopt_t);

    // parse args and opts
    const int8_t parse_status = argopt_parse(argc, argv, optc, optv);
    
    // check
    assert(parse_status == ARGOPT_PARSE_ERROR);
    assert(str_equals(cstr(rpath), "../temp"));
    assert(str_equals(cstr(wpath), "./docs/dw/data/"));
    assert(intensity == (float)0.7);
    assert(polarity == (float)0.35);
    assert(volume == 83);
    assert(level == 13);
    assert(update == 'y');
    assert(upgrade == 'y');

    // free resources
    str_free(rpath);
    str_free(wpath);

    DEBUG_DEFAULT_QUIT();
    return 0;
}
