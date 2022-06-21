#include <assert.h>

#include "../../inc/vita/util/argopt.h"

int main(void) {
	char *path = NULL;
	float test = 0.2;
	bool audio = false;
	int volume = 50;
	char on_off = 'y';

	// ./test_argopt -p=../temp -t 10.12 --volume=97 --on_off n -a
	const size_t argc = 2;
	const char *argv[] = {
		// "./test_argopt", "-p=../temp", "-t", "10.12", "--volume=97", "--on_off", "n", "-a"
		"./test_argopt", "--volume=97"
	};

	const argopt_t optv[] = {
		{ "--path", "-p", "path to file", path, dt_cstr },
		{ "--test", "-t", "test value", &test, dt_float },
		{ "--audio", "-a", "audio boolean", &audio, dt_bool },
		{ "--volume", "-v", "volume value", &volume, dt_int },
		{ "--on_off", "-s", "on_off switch", &on_off, dt_char }
	};
	const size_t optc = sizeof(optv)/sizeof(argopt_t);

	printf("\t--- BEFORE ---\n");
	argopt_print(optc, optv);

	// arguments parsing
	printf("\n");
	argopt_parse(argc, argv, optc, optv);

	printf("\t--- AFTER ---\n");
	argopt_print(optc, optv);

	return 0;
}
