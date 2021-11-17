#include <assert.h>

#include "../../include/vita/util/args.h"

int main(void) {
	str_t *path = str("my/folder");
	str_t *test = str("0.2");
	str_t *val = str("0.1");
	str_t *verbose = str("1");

	argopt_t *a = args_parse(
		0, 
		NULL,
		4, 
		"path|p", "path to files", path,
		"test|t", "test ratio", test,
		"val|l", "validation ratio", val,
		"verbose|v", "verbose output", verbose
	);
	args_free(a);

	printf("%s --- %s --- %s\n", cstr(path), cstr(test), cstr(val));

	return 0;
}
































