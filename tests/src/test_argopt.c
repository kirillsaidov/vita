#include <assert.h>

#include "../../inc/vita/util/argopt.h"

int32_t main(void) {
	// strings
	str_t *rpath = NULL;
	str_t *wpath = NULL;

	// floats
	float intensity = 0.1;
	float polarity = 0.2;

	// bools
	bool verbose = true;
	bool audio = false;

	// ints
	int32_t volume = 50;
	int32_t level = 1;

	// chars
	char update = 'n';
	char upgrade = 'n';

	// arguments
	const size_t argc = 10;
	const char *argv[] = {
		"./test_argopt", "-p", "../temp", "-t", "10.5", "--volume=83", "--on_off", "n", "-a", "unknown"
	};

	// options
	// argopt_t optv[] = {
	// 	{ "--path", "-p", "path to file", OPT(path), dt_str }, // FIXME
	// 	{ "--test", "-t", "test value", OPT(test), dt_float },
	// 	{ "--audio", "-a", "audio boolean", OPT(audio), dt_bool },
	// 	{ "--volume", "-v", "volume value", OPT(volume), dt_int },
	// 	{ "--on_off", "-s", "on_off switch", OPT(on_off), dt_char }
	// };
	// const size_t optc = sizeof(optv)/sizeof(argopt_t);

	// // parse args and opts
	// argopt_print(optc, optv);
	// argopt_parse(argc, argv, optc, optv);
	// printf("\n"); argopt_print(optc, optv);
	
	// // check
	// assert(str_equals(cstr(rpath), "../temp"));
	// assert(str_equals(cstr(rpath), "./docs/dw/data/"));
	// assert(intensity == 0.7);
	// assert(polarity == 0.35);
	// assert(volume == 83);
	// assert(level == 13);
	// assert(update == 'y');
	// assert(upgrade == 'y');

	// // free resources
	// str_free(path);
	// str_free();

	return 0;
}
