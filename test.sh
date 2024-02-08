#!/bin/bash

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Function to run a command in minishell and capture the output
run_test() {
    command=$1
    expected=$2
    echo "$command" | ./minishell > output.txt 2>&1
    actual=$(<output.txt)
    if [ "$actual" == "$expected" ]; then
        echo -e "${GREEN}Test passed: $command${NC}"
    else
        echo -e "${RED}Test failed: $command${NC}"
        echo "Expected: '$expected', but got: '$actual'"
    fi
    rm output.txt
}

# Setup a test directory for 'cd'
mkdir -p test_dir
# 'pwd' expected output
PWD_EXPECTED=$(pwd)

# Test cases
## invalid command
# run_test "hello" "minishell$ hello
# minishell: hello: command not found
# minishell$ exit"

echo with option -n
run_test "echo -n Hello, world!" "Hello, world!"
## cd with only a relative or absolute path
# run_test "cd test_dir; pwd" "${PWD_EXPECTED}/test_dir"
# run_test "cd ..; pwd" "$(dirname "${PWD_EXPECTED}")"
# ## pwd with no options
# run_test "pwd" "${PWD_EXPECTED}"
# ## export with no options (assuming it prints all environment variables, so we just check for a known var)
# run_test "export" "$(printenv | grep HOME)"
# ## unset with no options (unset a variable and then try to access it)
# run_test "export MY_VAR=123; unset MY_VAR; echo \$MY_VAR" ""
# ## env with no options or arguments (just check it doesn't error out, might need adjustment based on output)
# run_test "env" "$(env)"
# ## exit with no options (check for successful exit code)
# run_test "exit" ""

# Cleanup test directory
rm -rf test_dir
