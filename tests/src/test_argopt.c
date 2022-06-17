#include <assert.h>

#include "../../inc/vita/util/argopt.h"

int main(void) {
	str_t *path = NULL;
	float test = 0.2;
	int volume = 50;
	char on_off = 'y';

	argopt2_t opt[] = {
		{ "path", "p", "path to file", path, dt_str },
		{ "test", "t", "test value", &test, dt_float },
		{ "volume", "v", "volume value", &volume, dt_float },
		{ "on_off", "s", "on_off switch", &on_off, dt_char }
	};

	argopt_print(opt, 4);


	// str_t *path = str("my/folder");
	// str_t *test = str("0.2");
	// str_t *val = str("0.1");
	// str_t *verbose = str("1");

	// argopt_t *a = args_parse(
	// 	0,
	// 	NULL,
	// 	4,
	// 	"path|p", "path to files", path,
	// 	"test|t", "test ratio", test,
	// 	"val|l", "validation ratio", val,
	// 	"verbose|v", "verbose output", verbose
	// );

	// assert(plist_len(a->optionLong) == 4);
	// assert(plist_len(a->optionShort) == 4);
	// assert(plist_len(a->optionDesc) == 4);

	// printf("%s\n%s\n", cstr(plist_get(a->optionLong, 1)), cstr(plist_get(a->optionDesc, 1)));

	// args_free(a);
	// str_free(path);
	// str_free(test);
	// str_free(val);
	// str_free(verbose);

	return 0;
}
