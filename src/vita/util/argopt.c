#include "vita/util/argopt.h"

static void argopt_parse_eq(const char *const arg, const size_t optc, argopt_t *const optv);
static void argopt_parse_without_eq(size_t *counter, const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv);
static void argopt_assign_value(argopt_t *const opt, const char *const value);

bool argopt_parse(const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv) {
    if(argc < 2 && optc < 1 && argv == NULL && optv == NULL) {
        return false;
    }

    // parse argument options (starting from 1 and skipping the binary name)
    for (size_t i = 0; i < argc; i++) {
        const char *const arg = argv[i];

        // if we can find "="
        if(strstr(arg, "=") != NULL) {
            argopt_parse_eq(arg, optc, optv);
        } else {
            argopt_parse_without_eq(&i, argc, argv, optc, optv);
        }
    }

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
            case dt_cstr:
                printf("%s|", opt.optionValue == NULL ? "" : cstr(opt.optionValue)); 
                break;
            default:
                break;
        }

        printf("%s\n", dt_to_str(opt.optionType));
    }
}

static void argopt_parse_eq(const char *const arg, const size_t optc, argopt_t *const optv) {
    str_t *sarg_value = str(arg);
    str_t *s_eq_split = str_pop_get_first(NULL, sarg_value, "=");
    
    // find the corresponding option in optv
    for(size_t i = 0; i < optc; i++) {
        argopt_t *const opt = &optv[i];

        // check long and short options
        if(str_equals(cstr(s_eq_split), opt->optionLong) || str_equals(cstr(s_eq_split), opt->optionShort)) {
            argopt_assign_value(opt, cstr(sarg_value));
            break;
        }
    }

    str_free(sarg_value);
    str_free(s_eq_split);
}

static void argopt_parse_without_eq(size_t *counter, const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv) {
    str_t *sarg = str(argv[*counter]);

    // find the corresponding option
    for(size_t i = 0; i < optc; i++) {
        argopt_t *const opt = &optv[i];

        // check long and short options
        if(str_equals(cstr(sarg), opt->optionLong) || str_equals(cstr(sarg), opt->optionShort)) {
            if(*counter + 1 < argc) {
                argopt_assign_value(opt, argv[1 + (*counter)++]);
            } else {
                argopt_assign_value(opt, "true");
            }

            break;
        }
    }

    str_free(sarg);
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
        case dt_cstr:
            opt->optionValue = str(value);
            break;
        default:
            break;
    }
}



















