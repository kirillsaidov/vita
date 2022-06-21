#include "vita/util/argopt.h"

static void argopt_parse_eq(size_t *argv_counter, const size_t argc, const char **const argv, const size_t optc, const argopt_t *optv);
// static void argopt_parse_without_eq(const argopt_t *opt, const char **const argv);
static void argopt_assing_value(argopt_t *const opt, const char *const value);

bool argopt_parse(const size_t argc, const char **const argv, const size_t optc, const argopt_t *const optv) {
    if(argc < 2 && optc < 1 && argv == NULL && optv == NULL) {
        return false;
    }

    // parse argument options (starting from 1 and skipping the binary name)
    for (size_t i = 0; i < argc; i++) {
        const char *const arg = argv[i];

        // if we can find "="
        if(strstr(arg, "=") != NULL) {
            argopt_parse_eq(&i, argc, argv, optc, optv);
        } else {

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
                printf("%s|", opt.optionValue == NULL ? ("") : opt.optionValue);
                break;
            default:
                break;
        }

        printf("%s\n", dt_to_str(opt.optionType));
    }
}

static void argopt_parse_eq(size_t *argv_counter, const size_t argc, const char **const argv, const size_t optc, const argopt_t *optv) {
    str_t *sarg_value = str(argv[*argv_counter]);
    str_t *s_eq_split = str_pop_get_first(NULL, sarg_value, "=");
    
    // find the corresponding option in optv
    for(size_t i = 0; i < optc; i++) {
        const argopt_t *opt = &optv[i];

        // check long and short options
        if(str_equals(cstr(s_eq_split), opt->optionLong) || str_equals(cstr(s_eq_split), opt->optionShort)) {
            printf("arg: %s\n", argv[*argv_counter]);
            printf("opt: %s|%s\n", cstr(s_eq_split), cstr(sarg_value));
            argopt_assing_value(opt->optionValue, cstr(sarg_value));
            break;
        }
    }

    str_free(sarg_value);
    str_free(s_eq_split);
}

static void argopt_assing_value(argopt_t *const opt, const char *const value) {
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
            // *(int*)(opt->optionValue) = dt_int_val;
            memcpy(opt->optionValue, &dt_int_val, sizeof(int));
            break;
        case dt_float:
            dt_float_val = strtof(value, NULL);
            // memcpy(opt->optionValue, &dt_float_val, sizeof(float));
            // *(float*)(opt->optionValue) = dt_float_val;
            break;
        case dt_bool:
            dt_bool_val = (str_equals(value, "true") || value[0] == '1' ? true : false);
            *(bool*)(opt->optionValue) = dt_bool_val;
            break;
        case dt_char:
            *(char*)(opt->optionValue) = value[0];
            break;
        case dt_cstr:
            opt->optionValue = (char*)value;
            break;
        default:
            break;
    }
}



/*{
    // temporary variables
    int dt_int_val = 0;
    float dt_float_val = 0;
    bool dt_bool_val = false;
    str_t dt_str_val = {0};

    // check if we have "=" case, split
    str_t sarg = str_make_on_stack(arg);
    if(str_can_find(&sarg, "=")) {
        s_eq_split = str_pop_get_first(NULL, &sarg, "=");

        // check long and short options, save the value
        if(str_equals(cstr(s_eq_split), opt.optionLong) && str_equals(cstr(s_eq_split), opt.optionShort)) {
            switch(opt.optionType) {
                case dt_int:
                    dt_int_val = strtol(arg + (str_len(s_eq_split) + 1) * sizeof(char), NULL, 10);
                    *(int*)(opt.optionValue) = dt_int_val;
                    // memcpy(opt.optionValue, &dt_int_val, sizeof(int));
                    break;
                case dt_float:
                    dt_float_val = strtof(arg + (str_len(s_eq_split) + 1) * sizeof(char), NULL);
                    *(float*)(opt.optionValue) = dt_float_val;
                    // memcpy(opt.optionValue, &dt_float_val, sizeof(float));
                    break;
                case dt_bool:
                    dt_str_val = str_make_on_stack(arg + (str_len(s_eq_split) + 1) * sizeof(char));
                    *(bool*)(opt.optionValue) = str_can_find(&dt_str_val, "true") ? true : false;
                    // memcpy(opt.optionValue, &dt_bool_val, sizeof(bool));
                    break;
                case dt_char:
                    *(char*)(opt.optionValue) = (arg + (str_len(s_eq_split) + 1) * sizeof(char))[0];
                    // memcpy(opt.optionValue, (arg + (str_len(s_eq_split) + 1) * sizeof(char)), sizeof(char));
                    break;
                case dt_str:
                    *(str_t*)(opt.optionValue) = str_make_on_stack(arg + (str_len(s_eq_split) + 1) * sizeof(char));
                    break;
                default:
                    break;
            }
        }
    } 
}*/




















