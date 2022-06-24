#include <assert.h>

#include "../../inc/vita/util/argopt.h"

int main(void) {
	str_t *path = NULL;
	float test = 0.2;
	bool audio = false;
	int volume = 50;
	char on_off = 'y';

	// arguments
	const size_t argc = 10;
	const char *argv[] = {
		"./test_argopt", "-p", "../temp", "-t", "10.5", "--volume=83", "--on_off", "n", "-a", "unknown"
	};

	// options
	argopt_t optv[] = {
		{ "--path", "-p", "path to file", OPT(path), dt_str }, // FIXME
		{ "--test", "-t", "test value", OPT(test), dt_float },
		{ "--audio", "-a", "audio boolean", OPT(audio), dt_bool },
		{ "--volume", "-v", "volume value", OPT(volume), dt_int },
		{ "--on_off", "-s", "on_off switch", OPT(on_off), dt_char }
	};
	const size_t optc = sizeof(optv)/sizeof(argopt_t);

	// parse args and opts
	argopt_print(optc, optv);
	argopt_parse(argc, argv, optc, optv);
	printf("\n"); argopt_print(optc, optv);
	
	// assert(optv[0].optionValue != NULL);
	// printf("[%s]\n", cstr(optv[0].optionValue)); 
	// printf("[%s]\n", "../temp");
	// check
	// assert(str_equals(cstr(path), "../temp"));
	// assert(test == 10.5);
	// assert(audio == true);
	// assert(volume == 83);
	// assert(on_off == 'n');

	// free resources
	// str_free(path);

	return 0;
}
