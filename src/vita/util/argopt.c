#include "vita/util/argopt.h"

static void argopt_assign_value(argopt_t *const opt, const char *const value);

bool argopt_parse(const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv) {
    if(argc < 2 && optc < 1 && argv == NULL && optv == NULL) {
        return false;
    }

    // parse argument options (skipping the binary name)
    const char *unrecognized_option = NULL;
    for (size_t i = 1; i < argc; i++) {
        str_t *s_arg_value = str(argv[i]);
        str_t *s_opt_split = str_pop_get_first(NULL, s_arg_value, "=");

        // find the corresponding option in optv
        for(size_t j = 0; j < optc; j++) {
            argopt_t *const opt = &optv[j];

            /* Check for 2 cases:
                1. Parsing with "="
                2. Parsing without "="
            */

            // save argv[i] as unrecognized_option
            unrecognized_option = argv[i];

            // Case 1
            if(s_opt_split != NULL) { 
                if(str_equals(cstr(s_opt_split), opt->optionLong) || str_equals(cstr(s_opt_split), opt->optionShort)) {
                    argopt_assign_value(opt, cstr(s_arg_value));

                    // reset to NULL, since it was recognized
                    unrecognized_option = NULL;
                    break;
                }
            } else {
                // Case 2:
                if(str_equals(cstr(s_arg_value), opt->optionLong) || str_equals(cstr(s_arg_value), opt->optionShort)) {
                    if(i + 1 < argc && argv[i+1][0] != '-') {
                        argopt_assign_value(opt, argv[++i]);
                    } else {
                        argopt_assign_value(opt, "true");
                    }

                    // reset to NULL, since it was recognized
                    unrecognized_option = NULL;
                    break;
                }
            }
        }
        
        // free resources
        str_free(s_arg_value);
        str_free(s_opt_split);

        // unrecognized option
        if(unrecognized_option != NULL) {
            printf("Unrecognized option: %s!\n", unrecognized_option);
            break;
        }
    }

    return true;
}

void argopt_print_help(const char *header, const char *footer, const size_t optc, const argopt_t *const optv) {
    if(optc < 1 || optv == NULL) {
        return;
    }
    
    // calculate optionLong and optionShort padding
    int32_t olPadding = 0;
    int32_t osPadding = 0;
    for(size_t i = 0; i < optc; i++) {
        olPadding = MAX(strlen(optv[i].optionLong), olPadding);
        osPadding = MAX(strlen(optv[i].optionShort), osPadding);
    }

    // print header
    if(header != NULL) {
        printf("%s\n", header);
    }

    // print help (usage) manual
    for(size_t i = 0; i < optc; i++) {
        printf("%*s %*s %s\n", osPadding, optv[i].optionShort, olPadding, optv[i].optionLong, optv[i].optionDesc);
    }

    // print footer
    if(footer != NULL) {
        printf("%s\n", footer);
    }
}

static void argopt_assign_value(argopt_t *const opt, const char *const value) {
    if(opt == NULL || value == NULL) {
        return;
    }

    // temp value place holders
    int32_t ivalue = 0;
    float fvalue = 0.0;
    bool bvalue = false;

    // handling str_t (we need to check if it has been initialized with default value)
    str_t **svalue = (str_t**)opt->optionValue;

    // save arg value
    switch(opt->optionType) {
        case dt_int:
            ivalue = (int32_t)strtol(value, NULL, 10);
            *(int32_t*)(opt->optionValue) = ivalue;
            break;
        case dt_float:
            fvalue = strtof(value, NULL);
            *(float*)(opt->optionValue) = fvalue;
            break;
        case dt_bool:
            bvalue = (str_equals(value, "true") || value[0] == '1' ? true : false);
            *(bool*)(opt->optionValue) = bvalue;
            break;
        case dt_char:
            *(char*)(opt->optionValue) = value[0];
            break;
        case dt_str:
            if(*svalue == NULL) {
                *(str_t**)opt->optionValue = str(value);
            } else {
                str_clear(*svalue);
                str_append(*svalue, value);
            }
            break;
        default:
            break;
    }
}



















