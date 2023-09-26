#include <assert.h>

#include "../../inc/vita/util/argopt.h"

int32_t main(void) {
    vt_mallocator_t *alloctr = vt_mallocator_create();

    // strings
    vt_str_t *rpath = vt_str_create("my/temp/folder/default/initialized", alloctr);
    vt_str_t *wpath = NULL;

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
    vt_argopt_t optv[] = {
        { "--rpath", "-r", "read path", VT_ARGOPT(rpath), VT_TYPE_STR },
        { "--wpath", "-w", "save path", VT_ARGOPT(wpath), VT_TYPE_STR },

        { "--intensity", "-i", "level of intensity between [0; 1]", VT_ARGOPT(intensity), VT_TYPE_FLOAT },
        { "--polarity", "-p", "level of polarity between [0; 1]", VT_ARGOPT(polarity), VT_TYPE_FLOAT },

        { "--verbose", "-v", "verbose output", VT_ARGOPT(verbose), VT_TYPE_BOOL },
        { "--audio", "-a", "include audio", VT_ARGOPT(audio), VT_TYPE_BOOL },

        { "--volume", "-vl", "volume level", VT_ARGOPT(volume), VT_TYPE_INT32 },
        { "--level", "-l", "output level", VT_ARGOPT(level), VT_TYPE_INT32 },

        { "--update", "-ud", "update (y, n)", VT_ARGOPT(update), VT_TYPE_CHAR },
        { "--upgrade", "-ug", "upgrade (y, n)", VT_ARGOPT(upgrade), VT_TYPE_CHAR }
    };
    const size_t optc = sizeof(optv)/sizeof(vt_argopt_t);

    // parse args and opts
    const int8_t parse_status = vt_argopt_parse(argc, argv, optc, optv, alloctr);
    
    // check
    assert(parse_status == VT_ARGOPT_PARSE_ERROR);
    assert(vt_str_equals_z(vt_str_z(rpath), "../temp"));
    assert(vt_str_equals_z(vt_str_z(wpath), "./docs/dw/data/"));
    assert(intensity == (float)0.7);
    assert(polarity == (float)0.35);
    assert(volume == 83);
    assert(level == 13);
    assert(update == 'y');
    assert(upgrade == 'y');

    // free resources
    vt_str_destroy(rpath);
    vt_str_destroy(wpath);

    vt_mallocator_destroy(alloctr);
    return 0;
}
