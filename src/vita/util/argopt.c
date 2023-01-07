#include "vita/util/argopt.h"

static void argopt_assign_value(argopt_t *const opt, const char *const value);

int8_t argopt_parse(const size_t argc, const char **const argv, const size_t optc, argopt_t *const optv) {
    if(argc < 2 && optc < 1 && argv == NULL && optv == NULL) {
        DEBUG_ASSERT(argc >= 2, "argc must be >= 2!");
        DEBUG_ASSERT(optc >= 1, "optc must be >= 1!");
        DEBUG_ASSERT(argv != NULL, "argv is NULL!");
        DEBUG_ASSERT(optv != NULL, "optv is NULL!");
        return false;
    }

    // parse status
    int8_t parse_status = ARGOPT_PARSE_SUCCESS;

    // if help wanted
    if(argc == 2 && (argv[1][0] == '?' || str_equals(argv[1], "-h") || str_equals(argv[1], "--help"))) {
        return ARGOPT_PARSE_HELP_WANTED;
    }

    // parse argument options (skipping the binary name)
    str_t *s_arg_value = strn_empty(DEFAULT_INIT_ELEMENTS);
    str_t *s_opt_split = strn_empty(DEFAULT_INIT_ELEMENTS);
    const char *unrecognized_option = NULL;
    for(size_t i = 1; i < argc; i++) {
        // save argv[i] as unrecognized_option
        unrecognized_option = argv[i];

        // split 'option=value' by '='
        str_append(s_arg_value, argv[i]);
        s_opt_split = str_pop_get_first(s_opt_split, s_arg_value, "=");

        // find the corresponding option in optv
        for(size_t j = 0; j < optc; j++) {
            argopt_t *const opt = &optv[j];

            /* Check for 2 cases:
                1. Parsing with "="     | 'option=value'
                2. Parsing without "="  | 'option value'
            */

            // Case 1
            if(str_len(s_opt_split)) {
                // check if option is known
                if(str_equals(cstr(s_opt_split), opt->optionLong) || str_equals(cstr(s_opt_split), opt->optionShort)) {
                    argopt_assign_value(opt, cstr(s_arg_value));

                    // reset to NULL, since it was recognized
                    unrecognized_option = NULL;
                    break;
                }
            } else { // Case 2:
                // check if option is known
                if(str_equals(cstr(s_arg_value), opt->optionLong) || str_equals(cstr(s_arg_value), opt->optionShort)) {
                    if(i + 1 < argc && argv[i+1][0] != '-') {
                        argopt_assign_value(opt, argv[++i]);
                    } else {
                        argopt_assign_value(opt, "1"); // if it's a boolean | '--verbose'
                    }

                    // reset to NULL, since it was recognized
                    unrecognized_option = NULL;
                    break;
                }
            }
        }

        // if an option wasn't unrecognized, return
        if(unrecognized_option != NULL) {
            fprintf(stdout, "Unrecognized option: %s!\n", unrecognized_option);
            parse_status = ARGOPT_PARSE_ERROR;
            break;
        }

        // reset to zero
        str_clear(s_arg_value);
        str_clear(s_opt_split);
    }

    // free resources
    str_free(s_arg_value);
    str_free(s_opt_split);

    return parse_status;
}

void argopt_print_help(const char *header, const char *footer, const size_t optc, const argopt_t *const optv) {
    if(optc < 1 || optv == NULL) {
        DEBUG_ASSERT(optc >= 1, "optc must be >= 1!");
        DEBUG_ASSERT(optv != NULL, "optv is NULL!");
        return;
    }
    
    // calculate optionLong and optionShort padding
    int32_t olPadding = 6; // strlen("--help")
    int32_t osPadding = 2; // strlen("-h")
    for(size_t i = 0; i < optc; i++) {
        olPadding = MAX((int32_t)strlen(optv[i].optionLong), olPadding);
        osPadding = MAX((int32_t)strlen(optv[i].optionShort), osPadding);
    }

    // print header
    if(header != NULL) {
        fprintf(stdout, "%s\n", header);
    }

    // print help (usage) manual
    for(size_t i = 0; i < optc; i++) {
        fprintf(stdout, "%*s %*s %s\n", osPadding, optv[i].optionShort, olPadding, optv[i].optionLong, optv[i].optionDesc);
    }
    fprintf(stdout, "%*s %*s %s\n", osPadding, "-h", olPadding, "--help", "This help information.");

    // print footer
    if(footer != NULL) {
        fprintf(stdout, "%s\n", footer);
    }
}

/* ------------------------ PRIVATE ------------------------ */

static void argopt_assign_value(argopt_t *const opt, const char *const value) {
    if(opt == NULL || value == NULL) {
        return;
    }

    // save arg value
    switch(opt->optionType) {
        // int
        case dt_int8:
            *(int8_t*)(opt->optionValue) = (int8_t)conv_str_to_int64(value);
            break;
        case dt_int16:
            *(int16_t*)(opt->optionValue) = (int16_t)conv_str_to_int64(value);
            break;
        case dt_int32:
            *(int32_t*)(opt->optionValue) = (int32_t)conv_str_to_int64(value);
            break;
        case dt_int64:
            *(int64_t*)(opt->optionValue) = conv_str_to_int64(value);
            break;
        
        // uint
        case dt_uint8:
            *(uint8_t*)(opt->optionValue) = (uint8_t)conv_str_to_uint64(value);
            break;
        case dt_uint16:
            *(uint16_t*)(opt->optionValue) = (uint16_t)conv_str_to_uint64(value);
            break;
        case dt_uint32:
            *(uint32_t*)(opt->optionValue) = (uint32_t)conv_str_to_uint64(value);
            break;
        case dt_uint64:
            *(uint64_t*)(opt->optionValue) = conv_str_to_uint64(value);
            break;
        
        // float
        case dt_float:
            *(float*)(opt->optionValue) = (float)conv_str_to_double(value);
            break;
        case dt_double:
            *(double*)(opt->optionValue) = conv_str_to_double(value);
            break;
        
        // bool, char, str, cstr
        case dt_bool:
            *(bool*)(opt->optionValue) = (value[0] == '1' || str_equals(value, "true") ? true : false);
            break;
        case dt_char:
            *(char*)(opt->optionValue) = value[0];
            break;
        case dt_str:
            {
                str_t **svalue = (str_t**)opt->optionValue;

                // check if we need to allocate
                if(*svalue == NULL) {
                    *svalue = str(value);
                } else {
                    str_clear(*svalue);
                    str_append(*svalue, value);
                }
            }
            break;
        case dt_cstr:
            {
                char **zvalue = (char**)opt->optionValue;

                // check if we need to allocate
                if(*zvalue == NULL) {
                    *zvalue = strdup(value);
                } else {
                    const size_t len = strlen(value);
                    const size_t zLen = strlen(*zvalue);

                    // check we need to reallocate
                    if(len > zLen) {
                        char *ztmp = realloc(*zvalue, len - zLen);
                        if(ztmp == NULL) {
                            DEBUG_PRINTF("Failed to reallocate cstr to assign a new value!\n");
                            return;
                        }

                        // save
                        *zvalue = ztmp;
                    }

                    // copy values
                    memcpy(*zvalue, value, len);
                }
            }
            break;
        default:
            break;
    }
}



















