#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../../include/vita.h"

void test(void* ptr);
array_pt strbuf_split(const strbuf_pt sb, const str_t sep);

int main(void) {
	// vita_init();
	vita_manual_collect(true);

	// assert(array_memhandler_internal() != NULL);

	/* WORKS 
	array_pt arr = array_new(3, sizeof(strbuf_pt));
	{
		strbuf_pt s1 = strbuf(str("hello, world!"));
		array_push(arr, s1);

		s1 = strbuf(str("what a beautiful day!"));
		array_push(arr, s1);
		
		s1 = strbuf(str("Love is a joke!"));
		array_push(arr, s1);
	
	}
	array_foreach(arr, test);*/

	strbuf_pt strSep = strbuf(str("hello world")); // test game string C birds animals
	// array_pt sblist = strbuf_split(strSep, str(" "));

	// printf("%zu\n", array_len(sblist));
	// array_foreach(sblist, test);


	strbuf_free(strSep);

	// vita_flush();
}

void test(void* ptr) {
	// printf("%s\n", ((strbuf_pt)ptr)->ptr);
	printf("FREED\n");
	strbuf_free(ptr);
}

array_pt strbuf_split(const strbuf_pt sb, const str_t sep) {
	if(is_null(sb)) {
		logger_warn(str("strbuf is NULL; exiting..."), str("strbuf_split"));
		return NULL;
	}

	// check if strbuf dependency 'array' is initialized
	if(is_null(array_memhandler_internal()) && !array_manual_collect_status()) {
		logger_error(str("strbuf_split depends on array_memhandler_internal. Either initialize it or do array_manual_collect(true)!"), str("strbuf_split"));
		return NULL;
	}

	// create an array of strbufs
	const size_t count = strbuf_contains(sb, sep) + 1;
	array_pt sbList = array_new(count, sizeof(strbuf_pt));
	if(is_null(sbList)) {
		logger_error(str("unable to create an array! exiting..."), str("strbuf_split"));
		return NULL;
	}

	// split the string by sep
	char* token = strtok((char*)strbuf_cstr(sb), as_cstr(sep));
	while(!is_null(token)) {
		strbuf_pt s = strbuf(str(token));
		array_push(sbList, s);

		printf("%s\n", s->ptr);

		token = strtok(NULL, as_cstr(sep));
	}

	// the problem here is that i need a 2d array, because I'm creating 
	// an array of strbuf pointers to char pointers
	

	return sbList;
}


















