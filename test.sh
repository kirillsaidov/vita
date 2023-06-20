#!/bin/bash

cd tests;
rm -rf bin;

# declare an array variable
declare -a tests=( \
    "test_core" \
    "test_mallocator" \
    "test_str" \
    "test_vec" \
    "test_plist" \
    "test_path" \
    "test_fileio" \
    "test_log" \
    "test_conv" \
    "test_argopt" \
    "test_math" \
    "test_debug" \
    "test_version" \
    "test_compiler" \
    "test_datetime" \
)

# colored output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

# now loop through the tests
for i in "${tests[@]}"
do
    # run the test
    make FILE=$i; make run FILE=$i

    # error checking
    ret_code=$?
    if [ $ret_code != 0 ]; then
        printf "${RED}# FAILED <$i>\n\n${NC}"
        exit $?
    fi

    printf "${GREEN}# PASSED <$i>\n\n${NC}"
done





