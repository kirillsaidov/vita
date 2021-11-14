#include <assert.h>

#include "../../include/vita/args.h"

int main(void) {
	char *path = "my/folder";
	char *test = "0.2";
	char *val = "0.1";
	char *verbose = "1";

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

	return 0;
}
































