#include "vita/util/argopt.h"

// static argopt_t * argopt_new(const size_t numOptions);
// static inline void argopt_free(argopt_t *a);
// static inline void argopt_str_free(void *ptr, const size_t index);
// static inline void argopt_parse_command_line_args(const char *const cmdArg, str_t *s);

// argopt_t *args_parse(const int argc, const char **const argv, const size_t numOptions, ...) {
//     // no options provided
//     if(argc < 1 || numOptions < 1) {
//         return NULL;
//     }

//     // allocate memory for argopt_t container
//     argopt_t *a = argopt_new(numOptions);
//     if(a == NULL) {
//         return NULL;
//     }

//     // find all arguments
//     plist_t *ptemp = plist_create(2); {
//         str_t *stemp = str(""); {
//             va_list v;
//             va_start(v, numOptions);
//             for(int i = 0; i < numOptions; i++) {
//                 // get options
//                 char *csOption = va_arg(v, char*);
//                 char *csDesc = va_arg(v, char*);
//                 str_t *sval = va_arg(v, str_t*);

//                 // PARSE OPTION SHORT AND LONG
//                 str_append(stemp, csOption); {
//                     ptemp = str_split(ptemp, stemp, "|");

//                     // add optionShort and optionLong
//                     plist_push(a->optionShort, plist_pop_get(ptemp));
//                     plist_push(a->optionLong, plist_pop_get(ptemp));
//                 } str_clear(stemp);

//                 // PARSE OPTION DESCRIPTION
//                 plist_push(a->optionDesc, str(csDesc));

//                 // PARSE OPTION VALUE
//                 // ...
//             }
//             va_end(v);
//         } str_free(stemp);
//     } plist_destroy(ptemp);

//     return a;
// }

// void args_free(argopt_t *a) {
//     argopt_free(a);
// }

// static argopt_t * argopt_new(const size_t numOptions) {
//     argopt_t *a = malloc(sizeof(argopt_t));
//     if(a == NULL) {
//         return NULL;
//     }

//     // allocate memory for arguments
//     *a = (argopt_t) {
//         .optionLong = plist_create(numOptions),
//         .optionShort = plist_create(numOptions),
//         .optionDesc = plist_create(numOptions),
//         .helpInfo = strn(numOptions),
//     };
//     if(
//         a->optionLong == NULL ||
//         a->optionShort == NULL ||
//         a->optionDesc == NULL ||
//         a->helpInfo == NULL
//     ) {
//         // free args
//         plist_destroy(a->optionLong);
//         plist_destroy(a->optionShort);
//         plist_destroy(a->optionDesc);
//         str_free(a->helpInfo);

//         // free argopt_t
//         free(a);
//         a = NULL;
//     }

//     return a;
// }

// static inline void argopt_free(argopt_t *a) {
//     // optionLong
//     plist_foreach(a->optionLong, argopt_str_free);
//     plist_destroy(a->optionLong);

//     // optionShort
//     plist_foreach(a->optionShort, argopt_str_free);
//     plist_destroy(a->optionShort);

//     // optionDesc
//     plist_foreach(a->optionDesc, argopt_str_free);
//     plist_destroy(a->optionDesc);

//     // helpInfo
//     str_free(a->helpInfo);

//     // free argopt_t
//     free(a);
//     a = NULL;
// }

// static inline void argopt_str_free(void *ptr, const size_t index) {
//     str_free(ptr);
// }

// static inline void argopt_parse_command_line_args(const char *const cmdArg, str_t *s) {

// }

bool argopt_parse(const int argc, const char **const argv, const argopt2_t *const options, const size_t numOptions) {
    return true;
}

void argopt_print(const argopt2_t *const options, const size_t numOptions) {
    for(size_t i = 0; i < numOptions; i++) {
        const argopt2_t a = options[i];

        printf("%s|%s|%s|", a.optionLong, a.optionShort, a.optionDesc);

        switch(a.optionType) {
            case dt_int:
                printf("%d|", *(int*)(a.optionValue));
                break;
            case dt_float:
                printf("%f|", *(float*)(a.optionValue));
                break;
            case dt_char:
                printf("%c|", *(char*)(a.optionValue));
                break;
            case dt_str:
                printf("%s|", (a.optionValue == NULL ) ? ("") : cstr((str_t*)(a.optionValue)));
                break;
            default:
                break;
        }

        printf("%s\n", dt_to_str(a.optionType));
    }
}
























