#include "vita/util/argopt.h"

static bool vt_argopt_validate(const size_t optc, const vt_argopt_t *const optv);
static void vt_argopt_assign_value(vt_argopt_t *const opt, const char *const value, struct VitaBaseAllocatorType *const alloctr);

int8_t vt_argopt_parse(const size_t argc, const char **const argv, const size_t optc, vt_argopt_t *const optv, struct VitaBaseAllocatorType *const alloctr) {
    // check for invalid input
    VT_ENFORCE(optv != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(optc >= 1, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(argv != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(argc >= 1, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // validate artopt
    if(!vt_argopt_validate(optc, optv)) {
        return VT_ARGOPT_PARSE_ERROR;
    }

    // parse status
    int8_t parse_status = VT_ARGOPT_PARSE_SUCCESS;

    // if help wanted
    if(argc == 2 && (argv[1][0] == '?' || vt_str_equals(argv[1], "-h") || vt_str_equals(argv[1], "--help"))) {
        return VT_ARGOPT_PARSE_HELP_WANTED;
    }

    // parse argument options (skipping the binary name)
    vt_str_t *s_arg_value = vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    vt_str_t *s_opt_split = vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    const char *unrecognized_option = NULL;
    for(size_t i = 1; i < argc; i++) {
        // save argv[i] as unrecognized_option
        unrecognized_option = argv[i];

        // split 'option=value' by '='
        vt_str_append(s_arg_value, argv[i]);
        s_opt_split = vt_str_pop_get_first(s_opt_split, s_arg_value, "=");

        // find the corresponding option in optv
        for(size_t j = 0; j < optc; j++) {
            vt_argopt_t *const opt = &optv[j];

            /* Check for 2 cases:
                1. Parsing with "="     | 'option=value'
                2. Parsing without "="  | 'option value'
            */

            // Case 1
            if(vt_str_len(s_opt_split)) {
                // check if option is known
                if(vt_str_equals(vt_str_z(s_opt_split), opt->optionLong) || vt_str_equals(vt_str_z(s_opt_split), opt->optionShort)) {
                    vt_argopt_assign_value(opt, vt_str_z(s_arg_value), alloctr);

                    // reset to NULL, since it was recognized
                    unrecognized_option = NULL;
                    break;
                }
            } else { // Case 2:
                // check if option is known
                if(vt_str_equals(vt_str_z(s_arg_value), opt->optionLong) || vt_str_equals(vt_str_z(s_arg_value), opt->optionShort)) {
                    if(i + 1 < argc && argv[i+1][0] != '-') {
                        vt_argopt_assign_value(opt, argv[++i], alloctr);
                    } else {
                        vt_argopt_assign_value(opt, "1", alloctr); // if it's a boolean | '--verbose'
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
            parse_status = VT_ARGOPT_PARSE_ERROR;
            break;
        }

        // reset to zero
        vt_str_clear(s_arg_value);
        vt_str_clear(s_opt_split);
    }

    // free resources
    vt_str_destroy(s_arg_value);
    vt_str_destroy(s_opt_split);

    return parse_status;
}

void vt_argopt_print_help(const char *header, const char *footer, const size_t optc, const vt_argopt_t *const optv) {
    // check for invalid input
    VT_ENFORCE(optv != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(optc >= 1, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    
    // calculate optionLong and optionShort padding
    int32_t olPadding = 6; // strlen("--help")
    int32_t osPadding = 2; // strlen("-h")
    for(size_t i = 0; i < optc; i++) {
        olPadding = vt_cmp_maxi32((int32_t)strlen(optv[i].optionLong), olPadding);
        osPadding = vt_cmp_maxi32((int32_t)strlen(optv[i].optionShort), osPadding);
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

static bool vt_argopt_validate(const size_t optc, const vt_argopt_t *const optv) {
    // check if options are valid
    for (size_t i = 0; i < optc; i++) {
        const vt_argopt_t *opt = &optv[i];

        // validate
        if(opt->optionLong == NULL)  { fprintf(stderr, "optionLong cannot be NULL!\n"); return false; }
        if(opt->optionShort == NULL) { fprintf(stderr, "optionShort cannot be NULL!\n"); return false; }
        if(opt->optionValue == NULL) { fprintf(stderr, "optionValue cannot be NULL!\n"); return false; }
        if(strnlen(opt->optionShort, 4) > 3) { fprintf(stderr, "optionShort=<%s> cannot be longer than 3 characters!\n", opt->optionShort); return false; }
        if(opt->optionType >= VT_TYPE_COUNT) { fprintf(stderr, "optionType specified is invalid!\n"); return false; }
    }
    
    return true;
}

static void vt_argopt_assign_value(vt_argopt_t *const opt, const char *const value, struct VitaBaseAllocatorType *const alloctr) {
    if(opt == NULL || value == NULL) {
        return;
    }

    // save arg value
    switch(opt->optionType) {
        // int
        case VT_TYPE_INT8:
            *(int8_t*)(opt->optionValue) = vt_conv_str_to_i8(value);
            break;
        case VT_TYPE_INT16:
            *(int16_t*)(opt->optionValue) = vt_conv_str_to_i16(value);
            break;
        case VT_TYPE_INT32:
            *(int32_t*)(opt->optionValue) = vt_conv_str_to_i32(value);
            break;
        case VT_TYPE_INT64:
            *(int64_t*)(opt->optionValue) = vt_conv_str_to_i64(value);
            break;
        
        // uint
        case VT_TYPE_UINT8:
            *(uint8_t*)(opt->optionValue) = vt_conv_str_to_u8(value);
            break;
        case VT_TYPE_UINT16:
            *(uint16_t*)(opt->optionValue) = vt_conv_str_to_u16(value);
            break;
        case VT_TYPE_UINT32:
            *(uint32_t*)(opt->optionValue) = vt_conv_str_to_u32(value);
            break;
        case VT_TYPE_UINT64:
            *(uint64_t*)(opt->optionValue) = vt_conv_str_to_u64(value);
            break;
        
        // floating point variables
        case VT_TYPE_FLOAT:
            *(float*)(opt->optionValue) = (float)vt_conv_str_to_f(value);
            break;
        case VT_TYPE_DOUBLE:
            *(double*)(opt->optionValue) = vt_conv_str_to_d(value);
            break;
        case VT_TYPE_REAL:
            *(real*)(opt->optionValue) = vt_conv_str_to_r(value);
            break;
        
        // bool, char, vt_str, vt_str_z
        case VT_TYPE_BOOL:
            *(bool*)(opt->optionValue) = (value[0] == '1' || vt_str_equals(value, "true") ? true : false);
            break;
        case VT_TYPE_CHAR:
            *(char*)(opt->optionValue) = value[0];
            break;
        case VT_TYPE_STR:
            {
                vt_str_t **svalue = (vt_str_t**)opt->optionValue;

                // check if we need to allocate
                if(*svalue == NULL) {
                    *svalue = vt_str_create(value, alloctr);
                } else {
                    vt_str_clear(*svalue);
                    vt_str_append(*svalue, value);
                }
            }
            break;
        case VT_TYPE_CSTR:
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
                            VT_DEBUG_PRINTF("%s: Failed to reallocate vt_str_z to assign a new value!\n", vt_status_to_str(VT_STATUS_ERROR_ALLOCATION));
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



















