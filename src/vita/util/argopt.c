#include "vita/util/argopt.h"

static void argopt_assign_value(argopt_t *const opt, const char *const value);

bool argopt_parse(const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv) {
    if(argc < 2 && optc < 1 && argv == NULL && optv == NULL) {
        return false;
    }

    // parse argument options (starting from 1 and skipping the binary name)
    str_t *s_arg_value = strn(DEFAULT_INIT_ELEMENTS);
    for (size_t i = 1; i < argc; i++) {
        str_append(s_arg_value, argv[i]);
        str_t *s_opt_split = str_pop_get_first(NULL, s_arg_value, "=");
    
        // find the corresponding option in optv
        for(size_t j = 0; j < optc; j++) {
            argopt_t *const opt = &optv[j];

            // if we can find "="
            if(s_opt_split != NULL) {
                // parse options with "="
                if(str_equals(cstr(s_opt_split), opt->optionLong) || str_equals(cstr(s_opt_split), opt->optionShort)) {
                    argopt_assign_value(opt, cstr(s_arg_value));
                    break;
                }
            } else {
                // parse without "="
                // if(str_equals(cstr(s_arg_value), opt->optionLong) || str_equals(cstr(s_arg_value), opt->optionShort)) {
                //     if(i + 1 < argc) {
                //         argopt_assign_value(opt, argv[++i]);
                //     } else {
                //         argopt_assign_value(opt, "true");
                //     }

                //     break;
                // }

                if(str_equals(cstr(s_arg_value), opt->optionLong) || str_equals(cstr(s_arg_value), opt->optionShort)) {
                    if(i + 1 < argc && opt->optionType != dt_bool) {
                        argopt_assign_value(opt, argv[++i]);
                    } else {
                        argopt_assign_value(opt, "true");
                    }

                    break;
                }
            }
        }

        str_clear(s_arg_value);
        str_free(s_opt_split);
    }

    str_free(s_arg_value);

    return true;
}

void argopt_print(const size_t optc, const argopt_t *const optv) {
    for(size_t i = 0; i < optc; i++) {
        const argopt_t opt = optv[i];

        printf("%s|%s|%s|", opt.optionLong, opt.optionShort, opt.optionDesc);

        switch(opt.optionType) {
            case dt_int:
                printf("%d|", *(int32_t*)(opt.optionValue));
                break;
            case dt_float:
                printf("%f|", *(float*)(opt.optionValue));
                break;
            case dt_bool:
                printf("%s|", *(bool*)(opt.optionValue) ? "true" : "false");
                break;
            case dt_char:
                printf("%c|", *(char*)(opt.optionValue));
                break;
            case dt_str:
                printf("%s|", *(str_t**)(opt.optionValue) == NULL ? "" : cstr(*(str_t**)(opt.optionValue))); 
                break;
            default:
                break;
        }

        printf("%s\n", dt_to_str(opt.optionType));
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
            bvalue = (!str_equals(value, "false") || value[0] == '1' ? true : false);
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



















