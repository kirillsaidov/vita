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

    // creates a strbuf (allocates)
    strbuf_pt sb = strbuf(str("This is a dynamic string!")); 
    
    printf("%s\n", strbuf_cstr(sb));
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

    // frees strbuf
    strbuf_free(sb); 

    return 0;
}






