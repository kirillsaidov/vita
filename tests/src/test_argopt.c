#include <assert.h>

#include "../../inc/vita/util/argopt.h"

int32_t main(void) {
    DEBUG_DEFAULT_INIT;

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
        "./test_argopt", 									// 1 +
        "-r=../temp", "--wpath", "./docs/dw/data/",			// 3 +
        "--intensity", "0.7", "-p=0.35", "--verbose", "-a", // 5 +
        "-vl=83", "--level", "13", "--update", "y",         // 4 +
        "-ug=y", "-uo", "--unkownOption", "unkownValue"		// 5 = 18
    };
    const size_t argc = 18;

    // options
    argopt_t optv[] = {
        { "--rpath", "-r", "read path", OPT(rpath), dt_str },
        { "--wpath", "-w", "save path", OPT(wpath), dt_str },

        { "--intensity", "-i", "level of intensity between [0; 1]", OPT(intensity), dt_float },
        { "--polarity", "-p", "level of polarity between [0; 1]", OPT(polarity), dt_float },

        { "--verbose", "-v", "verbose output", OPT(verbose), dt_bool },
        { "--audio", "-a", "include audio", OPT(audio), dt_bool },

        { "--volume", "-vl", "volume level", OPT(volume), dt_int },
        { "--level", "-l", "output level", OPT(level), dt_int },

        { "--update", "-ud", "update (y, n)", OPT(update), dt_char },
        { "--upgrade", "-ug", "upgrade (y, n)", OPT(upgrade), dt_char }
    };
    const size_t optc = sizeof(optv)/sizeof(argopt_t);

    // parse args and opts
    argopt_parse(argc, argv, optc, optv);
    
    // check
    printf("HAHAHA ---- : %s\n", cstr(rpath));
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

    DEBUG_DEFAULT_QUIT;
    return 0;
}
