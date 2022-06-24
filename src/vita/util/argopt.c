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
            // printf("len: %zu, value: %s\n", str_len(s_opt_split), cstr(s_opt_split));
            if(s_opt_split != NULL) {
                // parse options with "="
                if(str_equals(cstr(s_opt_split), opt->optionLong) || str_equals(cstr(s_opt_split), opt->optionShort)) {
                    argopt_assign_value(opt, cstr(s_arg_value));
                    break;
                }
            } else {
                // parse without "="
                if(str_equals(cstr(s_arg_value), opt->optionLong) || str_equals(cstr(s_arg_value), opt->optionShort)) {
                    if(i + 1 < argc) {
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
                printf("%d|", *(int*)(opt.optionValue));
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
    int dt_int_val = 0;
    float dt_float_val = 0.0;
    bool dt_bool_val = false;

    // save arg value
    switch(opt->optionType) {
        case dt_int:
            dt_int_val = (int)strtol(value, NULL, 10);
            *(int*)(opt->optionValue) = dt_int_val;
            break;
        case dt_float:
            dt_float_val = strtof(value, NULL);
            *(float*)(opt->optionValue) = dt_float_val;
            break;
        case dt_bool:
            dt_bool_val = (!str_equals(value, "false") || value[0] == '1' ? true : false);
            *(bool*)(opt->optionValue) = dt_bool_val;
            break;
        case dt_char:
            *(char*)(opt->optionValue) = value[0];
            break;
        case dt_str:
            printf("been here...: value = %s\n", value);
            *(str_t**)opt->optionValue = str(value);
            break;
        default:
            break;
    }
}



















