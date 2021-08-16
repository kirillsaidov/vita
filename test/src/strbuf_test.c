#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#include "../../include/vita.h"

int main(void) {
    // we want to manually manage memory (false by default)
    // (at this point the programmer is responsible for freeing the allocate memory)
    strbuf_manual_collect(true); 
    array_manual_collect(true);

    // creates a strbuf (allocates)
    strbuf_pt sb = strbuf(str("This is a dynamic string!")); 
    
    printf("%s\n", as_cstr(*sb));
    printf("free memory: %zu\n", strbuf_has_space(sb));

    // shrinks size
    strbuf_shrink(sb);
    printf("%s\n", strbuf_cstr(sb));
    printf("free memory: %zu\n", strbuf_has_space(sb));

    // reserves memory
    strbuf_reserve(sb, 10);
    printf("%s\n", strbuf_cstr(sb));
    printf("free memory: %zu\n", strbuf_has_space(sb));

    // appends
    printf("\nAppending\n");
    strbuf_append(sb, str(" Yay!"));
    printf("%s\n", strbuf_cstr(sb));
    printf("free memory: %zu\n", strbuf_has_space(sb));

    // inserts
    printf("\nInserting\n");
    strbuf_insert(sb, str("[RANDOM MSG - dynamo dynamic dynomite]"), 0);
    printf("%s\n", strbuf_cstr(sb));
    printf("free memory: %zu\n", strbuf_has_space(sb));

    // removes by index
    printf("\nRemoving by index\n");
    strbuf_remove(sb, 8, 4);
    printf("%s\n", strbuf_cstr(sb));
    printf("free memory: %zu\n", strbuf_has_space(sb));

    // removes a substring
    printf("\nRemoving by substring\n");
    strbuf_remove_str(sb, str("dynamic "));
    strbuf_remove_str(sb, str("Yay!"));
    strbuf_remove_str(sb, str("RANDOM!")); // error: substring was not found
    printf("%s\n", strbuf_cstr(sb));
    printf("free memory: %zu\n", strbuf_has_space(sb));

    // removes all instances of a substring in strbuf
    printf("\nRemoving all instances of a substring\n");
    strbuf_remove_str_all(sb, str("dyn"));
    printf("%s\n", strbuf_cstr(sb));
    printf("free memory: %zu\n", strbuf_has_space(sb));
    
    // check if strbuf contains substring
    printf("\nstrbuf contains \'amic\'? ");
    printf("answer: %s\n", ((strbuf_contains(sb, str("amic"))) ? ("true") : ("false")));
	printf("strbuf contains \'bannana\'? ");
    printf("answer: %s\n", ((strbuf_contains(sb, str("banana"))) ? ("true") : ("false")));
    
    // split string by separator
    array_pt sblist = strbuf_split(sb, str(" "));
    printf("sblist len: %zu\n", array_len(sblist));
    printf("%s\n", strbuf_cstr(array_get(sblist, 0)));
    /*for(size_t i = 0; i < array_len(sblist); i++) {
        printf("%s\n", strbuf_cstr(array_get(sblist, i)));
    }*/
    
    // frees strbuf
    strbuf_free(sb);

	// for(size_t i = 0; i < array_len(sblist); i++) {
	//     strbuf_free(array_get(sblist, 0));
	// }
	// mem_free((void*)(array_get(sblist, 0)));
	// mem_free((void*)(array_get(sblist, 0)));
    array_free(sblist);

    return 0;
}














