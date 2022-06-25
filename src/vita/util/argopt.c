#include "vita/util/argopt.h"

static void argopt_assign_value(argopt_t *const opt, const char *const value);

bool argopt_parse(const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv) {
    if(argc < 2 && optc < 1 && argv == NULL && optv == NULL) {
        return false;
    }

    // parse argument options (skipping the binary name)
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

            // Case 1:
            if(s_opt_split != NULL) {
                if(str_equals(cstr(s_opt_split), opt->optionLong) || str_equals(cstr(s_opt_split), opt->optionShort)) {
                    argopt_assign_value(opt, cstr(s_arg_value));
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

                    break;
                }
            }
        }
        
        // free resources
        str_free(s_arg_value);
        str_free(s_opt_split);
    }

    return true;
}

void argopt_print_help(const char* header, const size_t optc, const argopt_t *const optv) {}

void argopt_print_opts(const size_t optc, const argopt_t *const optv, int32_t padding) {
    if(optc < 1 || optv == NULL) {
        return;
    }

    // auto-set padding
    if(padding == -1) {
        padding = 13;
    }

    // header
    printf("%*s|%*s|%*s|%*s| %s\n", padding, "optionLong", padding, "optionShort", padding, "optionValue", padding, "optionType", "optionDesc");
    
    // data
    for(size_t i = 0; i < optc; i++) {
        const argopt_t opt = optv[i];

        printf("%*s|%*s|", padding, opt.optionLong, padding, opt.optionShort);

        switch(opt.optionType) {
            case dt_int:
                printf("%*d|", padding, *(int32_t*)(opt.optionValue));
                break;
            case dt_float:
                printf("%*f|", padding, *(float*)(opt.optionValue));
                break;
            case dt_bool:
                printf("%*s|", padding, *(bool*)(opt.optionValue) ? "true" : "false");
                break;
            case dt_char:
                printf("%*c|", padding, *(char*)(opt.optionValue));
                break;
            case dt_str:
                printf("%*s|", padding, *(str_t**)(opt.optionValue) == NULL ? "" : cstr(*(str_t**)(opt.optionValue))); 
                break;
            default:
                break;
        }

        printf("%*s| %s\n", padding, dt_to_str(opt.optionType), opt.optionDesc);
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
            *(str_t**)opt->optionValue = str(value);
            break;
        default:
            break;
    }
}



















