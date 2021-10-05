#! /bin/sh

cd tests;

# declare an array variable
declare -a tests=("str_test" "vec_test" "mat_test")

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
		echo "${RED}# FAILED <$i>\n${NC}"
		exit $?
	fi

	echo "${GREEN}# PASSED <$i>\n${NC}"
done





