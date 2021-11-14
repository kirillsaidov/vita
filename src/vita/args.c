#include "vita/args.h"

static argopt_t * argopt_new(const size_t numOptions);
static void argopt_str_free(void *ptr, const size_t index);
static void argopt_free(argopt_t *a);

static argopt_t * argopt_new(const size_t numOptions) {
    argopt_t *a = malloc(sizeof(argopt_t));
    if(is_null(a)) {
        return NULL;
    }

    // allocate memory for arguments
    a->cmdLong = plist_create(numOptions);
    a->cmdShort = plist_create(numOptions);
    a->cmdDesc = plist_create(numOptions);
    a->helpManual = strn(numOptions);
    if(
        is_null(a->cmdLong) || 
        is_null(a->cmdShort) || 
        is_null(a->cmdDesc) || 
        is_null(a->helpManual)
    ) { 
        // free args
        plist_destroy(a->cmdLong);
        plist_destroy(a->cmdShort);
        plist_destroy(a->cmdDesc);
        str_free(a->helpManual);

        // free argopt_t
        free(a);
        a = NULL;
    }

    return a;
}

static void argopt_str_free(void *ptr, const size_t index) {
    str_free(ptr);
}

static void argopt_free(argopt_t *a) {
    // free args
    plist_foreach(a->cmdLong, argopt_str_free);
        plist_destroy(a->cmdLong);
    plist_foreach(a->cmdShort, argopt_str_free);
        plist_destroy(a->cmdShort);
    plist_foreach(a->cmdDesc, argopt_str_free);
        plist_destroy(a->cmdDesc);
    str_free(a->helpManual);

    // free argopt_t
    free(a);
    a = NULL;
}

argopt_t *args_parse(const int argc, const char **const argv, const size_t numOptions, ...) {
	// allocate memory for argopt_t container
    argopt_t *a = argopt_new(numOptions);
    if(is_null(a)) {
        vita_warn("unable to allocate memory!", __FUNCTION__);
        return NULL;
    }
    
    // find all arguments
    va_list v;
    va_start(v, numOptions);
    for(int i = 0; i < numOptions; i++) {
        // save arguments
        // str_split by '|', then save long and short
        
        const char* csCmd = va_arg(v, char*);
        const char* csDesc = va_arg(v, char*);
        const char* csVal = va_arg(v, char*);

        printf("%s --- %s --- %s\n", csCmd, csDesc, csVal);
    }
    va_end(v);

    return a;
}

void args_free(argopt_t *a) {
    //...
    argopt_free(a);
}