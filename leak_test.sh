#!/bin/bash

# Define ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
NO_COLOR='\033[0m' # No Color


PROGRAM="./minishell"
# Define your test commands as before
INPUT_PARAMS=(

    # Open file descriptors
    # "echo <"./test_files/infile_big" | echo <"./test_files/infile""
    # "cat <<EOF\nCurrent date: $(date)\nEOF"
    # "cat <<EOF1\nThis is the outer heredoc.\n\nEOF1"
    # "cat <<EOF\nThis is a test with an escaped variable: \$VAR\nEOF"
    # "cat <<EOF\nThis is a test without a trailing newlineEOF"
    # "cat <<EOF > /tmp/heredoc_test.txt\nThis content will be written to a file.\nEOF"
    # "cat <<EOF > /tmp/heredoc_complex_test.txt\nThis file was generated on: $(date)\nEOF"
    # "while read line; do echo \"Line: \$line\"; done <<EOF\nFirst line\nSecond line\nEOF"

    # # Memory leaks
    # "echo hello\"\"world"
    # "export EMPTY_VAR='' | echo \${EMPTY_VAR:-'Default Value'}"
    # "export SET_VAR='Set' | unset SET_VAR | echo \${SET_VAR:-'Was unset'}"
    # # "export MISSING_VAR | echo ${MISSING_VAR?'Error: Variable is unset.'}" # line 271: MISSING_VAR: Error: Variable is unset.


    # # Memory leaks and open file descriptors
    # "export VAR='World'; cat <<EOF\nHello, $VAR!\nEOF"
    # "cat <<EOF | grep 'test'\nThis is a test line.\nThis line does not match.\nEOF"
    # "export VAR='World'; cat <<'EOF'\nHello, $VAR!\nEOF"


    # echo
    "echo hello world" # Test 1
    "echo \"hello world\"" # Test 2
    "echo 'hello world'" # Test 3
    "echo hello'world'" # Test 4
    # "echo hello\"\"world" # Test 5
    # "echo ''" # Test 6
    # "echo \"\$PWD\"" # Test 7
    # "echo '\$PWD'" # Test 8
    # "echo \"aspas ->'\"" # Test 9
    # "echo \"aspas -> ' \"" # Test 10
    # "echo 'aspas ->\"'" # Test 11
    # "echo 'aspas -> \" '" # Test 12
    # "echo \"> >> < * ? [ ] | ; [ ] || && ( ) & # \$  <<\"" # Test 13
    # "echo '> >> < * ? [ ] | ; [ ] || && ( ) & # \$  <<'" # Test 14
    # "echo \"exit_code ->\$? user ->\$USER home -> \$HOME\"" # Test 15
    # "echo 'exit_code ->\$? user ->\$USER home -> \$HOME'" # Test 16
    # "echo \$" # Test 17
    # "echo \$?" # Test 18
    # "echo \$?HELLO" # Test 19
    # "echo Hello, World!" # Test 20 (no quotes)
    # "echo 'Hello, World!'" # Test 21 (single quotes)
    # "echo \"Hello, World!\""  # Test 22 (double quotes)
    # "echo Hello\, World\!"  # Test 23 (escaped characters)
    # "echo 'Hello, '\"World!\"" # Test 24 (single quotes with escaped double quotes)
    # "echo \$HOME" # Test 25 (variable expansion)
    # "echo \$((2 + 2))"  # Test 26 (arithmetic expansion)
    # "echo $(echo Nested)"  # Test 27 (command substitution)
    # "echo `echo Backticks`"  # Test 28 (backticks)
    # "echo; echo Separate"  # Test 29 (multiple commands)
    # "echo 'Single quote -> '\"'\"' <- Single quote'" # Test 30 (single quotes inside single quotes)
    # "echo \"Double quote -> '\"'\"' <- Double quote\"" # Test 31 (single quotes inside double quotes)
    # "echo Text > /tmp/output.txt" # Test 32 (output redirection)
    # "cat < /tmp/output.txt" # Test 33 (input redirection)
    # "echo First && echo Second" # Test 34 (AND list)
    # "echo First || echo Failed" # Test 35 (OR list)
    # "echo \"Nested -> \$(echo command)\"" # Test 36 (nested command substitution)
    # "echo 'A space -> '\"\ \"" # Test 37 (space inside quotes)
    # "echo \"A new\nline\"" # Test 38 (newline inside double quotes)
    # "echo -e \"Tab\tcharacter\"" # Test 39 (tab character)
    # "echo \"A quote ' inside a quote\"" # Test 40 (quote inside double quotes)
    # "echo 'A quote \" inside a quote'" # Test 41 (quote inside single quotes)

    # # exit
    # "exit" # Test 42 (no arguments)
    # "exit 0" # Test 43 (numeric argument)
    # "exit 1" # Test 44 (numeric argument)
    # "exit 255" # Test 45 (numeric argument)
    # "exit 256" # Test 46 (numeric argument)
    # "exit -1" # Test 47 (negative argument)
    # "exit 42" # Test 48 (numeric argument)
    # "exit +42" # Test 49 (positive argument)
    # "exit 9999999999" # Test 50 (large positive number)
    # "exit -9999999999" # Test 51 (large negative number)
    # "exit abc" # Test 52 (non-numeric argument)
    # "exit 42abc" # Test 53 (non-numeric argument)
    # "exit 42 58" # Test 54 (too many arguments)

    # # cd
    # "cd /"  # Test 55 (root directory)
    # "cd /tmp"  # Test 56 (existing directory)
    # "cd /var/log"  # Test 57 (existing directory)
    # "cd .."  # Test 58 (parent directory)
    # "cd ../.."  # Test 59 (grandparent directory)
    # "cd ./"  # Test 60 (current directory)
    # "cd -"  # Test 61 (previous directory)
    # "cd \$HOME"  # Test 62 (home directory)
    # "cd \$PWD"  # Test 63 (current directory)
    # "cd \"Directory With Spaces\""  # Test 64 (directory with spaces)
    # "cd Directory\\ With\\ Spaces"  # Test 65 (directory with spaces)
    # "cd ./subdir"  # Test 66 (subdirectory)
    # "cd subdir/anotherdir"  # Test 67 (subdirectory)
    # "cd nonexistentdir"  # Test 68 (nonexistent directory)
    # "cd /root"  # Test 69 (directory with restricted access)
    # "cd"  # Test 70 (no arguments)
    # "cd ~"  # Test 71 (home directory)
    # "cd ~/subdir"  # Test 72 (subdirectory)
    # "cd ///"  # Test 73 (redundant slashes)
    # "cd ..//subdir"  # Test 74 (redundant slashes)

    # # env
    # "env"  # Test 75 (no arguments)
    # "env | grep PATH"  # Test 76 (display PATH variable)
    # "env | grep HOME"  # Test 77 (display HOME variable)
    # "env -i"  # Test 78 (reset environment)
    # "env -i ls"  # Test 79 (reset environment and run command)
    # "env | sort"  # Test 80 (sort environment variables)
    # "env | wc -l"  # Test 81 (count environment variables)
    # "env -i env" # Test 82 (reset environment and display environment variables)
    # "env VAR1=test env"  # Test 83 (set variable and display environment)
    # "env VAR1=test VAR2=another_test env" # Test 84 (set multiple variables and display environment)
    # "env -i VAR1=test ls"  # Test 85 (reset environment and set variable for command)
    # "env | grep VAR1"  # Test 86 (display variable set in previous command)

    # # unset
    # "export TEST_VAR1=hello && unset TEST_VAR1 && env | grep TEST_VAR1"  # Test 87 (unset variable and check it's gone)
    # "export TEST_VAR2=world && unset TEST_VAR2 && echo \$TEST_VAR2"  # Test 88 (unset variable and check it's gone)
    # "export TEST_VAR3=testing && unset TEST_VAR3 TEST_VAR4 && env | grep TEST_VAR3"  # Unset multiple variables, check first is gone
    # "export TEST_VAR4=anotherTest && unset TEST_VAR3 TEST_VAR4 && env | grep TEST_VAR4"  # Unset multiple variables, check second is gone
    # "unset PATH && echo \$PATH"  # Unset PATH and check it's gone
    # "export TEST_VAR5=foo && export TEST_VAR6=bar && unset TEST_VAR5 TEST_VAR6 && env | grep TEST_VAR5"  # Unset two variables, check first is gone
    # "export TEST_VAR5=foo && export TEST_VAR6=bar && unset TEST_VAR5 TEST_VAR6 && env | grep TEST_VAR6"  # Unset two variables, check second is gone
    # "unset NOT_SET_VAR && echo 'Unset a variable that was never set'"  # Attempt to unset a variable that doesn't exist

    # # export
    # "export NEW_VAR=new_value"  # Set a new environment variable
    # "export NEW_VAR=new_value| echo \$NEW_VAR"  # Set and then print a new environment variable
    # "export PATH=updated_value"  # Update an existing environment variable
    # "export PATH= | echo \$EXISTING_VAR"  # Unset an existing environment variable's value
    # "export"  # Display all exported environment variables
    # "export VAR_WITH_SPACES='value with spaces'| echo \$VAR_WITH_SPACES"  # Set a variable with spaces in its value
    # "export NUM_VAR=123| echo \$NUM_VAR"  # Set a numeric environment variable
    # "export VAR_WITH_SPECIAL_CHARS='!@#\$%^&*()'| echo \$VAR_WITH_SPECIAL_CHARS"  # Set a variable with special characters
    # "export PATH=\$PATH:/new/path"  # Append to the PATH variable
    # "export -p"  # Display all exported variables (using -p option for portability)
    # "export VAR_TO_UNSET=to_be_unset| unset VAR_TO_UNSET| echo \$VAR_TO_UNSET"  # Unset a variable and confirm it's gone
    # "export VAR_WITH_EQUALS='equals==in_value'| echo \$VAR_WITH_EQUALS"  # Set a variable containing equals signs in its value
    # "export 'VAR_WITH_QUOTES=\"quoted_value\"'| echo \$VAR_WITH_QUOTES"  # Set a variable with quoted value
    # "export VAR_TO_BE_EXPORTED=value| env | grep VAR_TO_BE_EXPORTED"  # Export a variable and check it's in env
    # "export hello"
    # "export HELLO=123"
    # "export A-"
    # "export HELLO=123 A"
    # "export HELLO=\"123 A-\""
    # "export hello world"
    # "export HELLO-=123"
    # "export ="
    # "export 123"


    # # pwd
    # "pwd /"
    # "pwd ~"
    # "pwd .."
    # "pwd ../../"
    # "pwd /usr/bin"
    # "pwd /etc/"
    # "pwd usr/local/share"

    # # ls
    # "ls >./outfiles/outfile01"
    # "ls > ./outfiles/outfile01"
    # "echo hi > ./outfiles/outfile01 bye"
    # "ls >./outfiles/outfile01 >./outfiles/outfile02"
    # "ls >./outfiles/outfile01 >./test_files/invalid_permission"
    # "ls >\"./outfiles/outfile with spaces\""
    # "ls >\"./outfiles/outfile\"\"1\"\"2\"\"3\"\"4\"\"5\""
    # "ls >\"./outfiles/outfile01\" >./test_files/invalid_permission >\"./outfiles/outfile02\""
    # "ls >./test_files/invalid_permission >\"./outfiles/outfile01\" >./test_files/invalid_permission"


    # # Variables expansion
    # "echo \$HOME"  # Simple variable expansion
    # "echo \$PATH"  # Expand PATH variable
    # "echo \$USER"  # Expand USER variable
    # "export TEST_VAR='Hello World' | echo \$TEST_VAR"  # Set and expand custom variable export TEST_VAR='Hello World'; echo $TEST_VAR
    # "echo \${UNSET_VAR:-default}"  # Use default value for unset variable echo ${UNSET_VAR:-default}
    # "echo \${HOME:-/home/default}"  # Use default value for set variable (should print HOME value) echo ${HOME:-/home/default}
    # "export VAR='value' | echo \${VAR:+alt_value}"  # Use alternative value if VAR is set export VAR='value'; echo ${VAR:+alt_value}
    # "echo \$(date)"  # Command substitution echo $(date)
    # "echo \$(echo \${HOME})"  # Nested command substitution echo $(echo ${HOME})
    # "echo \$((2 + 2))"  # Arithmetic expansion echo $((2 + 2))
    # "echo \$((\$RANDOM % 5))"  # Arithmetic expansion with variable echo $((RANDOM % 5))
    # "export EMPTY_VAR='' | echo \${EMPTY_VAR:-'Default Value'}"  # Default value for empty variable export EMPTY_VAR=''; echo ${EMPTY_VAR:-'Default Value'}
    # "export SET_VAR='Set' | unset SET_VAR | echo \${SET_VAR:-'Was unset'}"  # Check unset variable export SET_VAR='Set'; unset SET_VAR; echo ${SET_VAR:-'Was unset'}
    # "echo \"Nested \${VAR:\${1:\${2:1}}}\""  # Nested braces (invalid, for error handling) export VAR='world'; echo "Nested ${VAR:${1:${2:1}}}"
    # "echo \${#HOME}"  # Length of variable
    # "echo \${HOME:0:4}"  # Substring of variable echo ${HOME:0:4}
    # "export VAR='Hello' | echo \${VAR^}"  # Uppercase first letter export VAR='hello'; echo ${VAR^}
    # "export VAR='hello' | echo \${VAR^^}"  # Uppercase all letters export VAR='hello'; echo ${VAR^^}
    # "export VAR='HELLO' | echo \${VAR,}"  # Lowercase first letter export VAR='HELLO'; echo ${VAR,}
    # "export VAR='HELLO' | echo \${VAR,,}"  # Lowercase all letters export VAR='HELLO'; echo ${VAR,,}
    # "export PATH_VAR='/usr/local/bin:/usr/bin:/bin' | echo \${PATH_VAR//:/\n}"  # Replace all colons with newlines export PATH_VAR='/usr/local/bin:/usr/bin:/bin'; echo ${PATH_VAR//:/\n}
    # "export MISSING_VAR | echo \${MISSING_VAR?'Error: Variable is unset.'}"  # Error if variable is unset export MISSING_VAR; echo ${MISSING_VAR?'Error: Variable is unset.'}


    # # Pipelines and Redirections
    # "echo 'Hello World' | wc -c"  # Simple pipeline
    # "cat /etc/passwd | grep root | wc -l"  # Pipeline with three commands
    # "echo 'Hello from shell' > /tmp/testfile.txt"  # Output redirection
    # "cat < /tmp/testfile.txt"  # Input redirection
    # "grep shell < /tmp/testfile.txt | sort > /tmp/sorted.txt"  # Combine input and output redirection with pipeline
    # "cat /etc/passwd | grep nologin | wc -l > /tmp/nologin_count.txt"  # Pipeline output redirection
    # "echo 'case insensitive' | tr '[:lower:]' '[:upper:]'"  # Use of tr command in pipeline
    # "cat /etc/passwd | awk -F: '{print \$1}' | sort | uniq"  # Complex pipeline with awk
    # "cat /dev/null > /tmp/emptyfile.txt"  # Truncate file
    # "echo 'append this text' >> /tmp/testfile.txt"  # Append redirection
    # "cat /etc/passwd | head -n 5"  # Pipeline with head command
    # "cat /etc/passwd | tail -n 5"  # Pipeline with tail command
    # "cat /etc/hosts | grep localhost | tee /tmp/localhost.txt"  # Use of tee for output branching
    # "cat < /tmp/testfile.txt | grep 'Hello' | sort -r > /tmp/sorted_greets.txt"  # Complex pipeline combining input and output redirection
    # "cat /nonexistentfile 2> /tmp/stderr.txt"  # Redirect stderr to a file
    # "cat access.log | awk '{print $1}' | sort | uniq -c | sort -nr | awk '{print $2, "requests:", $1}'"
    # "find . -type f | rev | cut -d. -f1 | rev | sort | uniq -c | sort -nr | awk '{print $2, "files:", $1}'"
    # "ps aux | sort -nk +4 | tail | awk '{print $11, "memory usage:", $4"%"}'"
    # "git log --pretty=format:%aN | sort | uniq -c | sort -nr | awk '{print $2, "lines changed:", $1}'"
    # "find . -type f | xargs -L1 dirname | sort | uniq -c | sort -nr | awk '{print $2, "files:", $1}'"
    # "netstat -ntu | awk '{print $5}' | cut -d: -f1 | sort | uniq -c | sort -nr | awk '{print $2, "connections:", $1}'"
    # "grep -rI "pattern" . | wc -l | sort -nr | awk '{print $2, "occurrences:", $1}'"
    # "grep -rI "pattern" . | cut -d: -f1 | uniq -c | sort -nr | awk '{print $2, "occurrences:", $1}'"


    # # File operations
    # "grep hi <./test_files/infile"
    # "grep hi \"<infile\" < ./test_files/infile"
    # "echo hi < ./test_files/infile bye bye"
    # "grep hi <./test_files/infile_big <./test_files/infile"
    # "echo <\"./test_files/infile\" \"bonjour       42\""
    # "cat <\"./test_files/file name with spaces\""
    # "cat <./test_files/infile_big ./test_files/infile"
    # "cat <\"1\"\"2\"\"3\"\"4\"\"5\""
    # "echo <\"./test_files/infile\" <missing <\"./test_files/infile\""
    # "echo <missing <\"./test_files/infile\" <missing"
    # "cat <\"./test_files/infile\""
    # "echo <\"./test_files/infile_big\" | cat <\"./test_files/infile\""
    # "echo <\"./test_files/infile_big\" | cat \"./test_files/infile\""
    # "echo <\"./test_files/infile_big\" | echo <\"./test_files/infile\""
    # "echo hi | cat <\"./test_files/infile\""
    # "echo hi | cat \"./test_files/infile\""
    # "cat <\"./test_files/infile\" | echo hi"
    # "cat <\"./test_files/infile\" | grep hello"
    # "cat <\"./test_files/infile_big\" | echo hi"
    # "cat <missing"
    # "cat <missing | cat"
    # "cat <missing | echo oi"
    # "cat <missing | cat <\"./test_files/infile\""
    # "echo <123 <456 hi | echo 42"

    # # Heredoc
    # "cat '<<' EOF"     # Basic heredoc usage
    # "export VAR='World'; cat <<EOF\nHello, \$VAR!\nEOF" # Heredoc with variable expansion
    # "cat <<EOF\nCurrent date: \$(date)\nEOF" # Heredoc with command substitution
    # "cat <<EOF1\nThis is the outer heredoc.\n$(cat <<EOF2\nThis is the inner heredoc.\nEOF2\n)\nEOF1" # Nested heredoc (if supported)
    # "cat <<EOF\nThis is a test with an escaped variable: \\\$VAR\nEOF" # Heredoc with escaping
    # "cat <<EOF\nThis is a test without a trailing newlineEOF"     # Heredoc without trailing newline
    # "cat <<EOF > /tmp/heredoc_test.txt\nThis content will be written to a file.\nEOF" # Heredoc with redirection
    # "cat <<EOF | grep 'test'\nThis is a test line.\nThis line does not match.\nEOF" # Heredoc with pipe
    # "export VAR='World'; cat <<'EOF'\nHello, \$VAR!\nEOF" # Heredoc with variable expansion disabled (using single quotes around the delimiter)
    # "cat <<EOF > /tmp/heredoc_complex_test.txt\nThis file was generated on: \$(date)\nEOF" # Complex heredoc with redirection and command substitution
    # "while read line; do echo \"Line: \$line\"; done <<EOF\nFirst line\nSecond line\nEOF" # Heredoc used with a loop

)

# Initialize counters
COUNTER=0
SUCCESS_COUNT=0
TOTAL_COUNT=0

for PARAM in "${INPUT_PARAMS[@]}"; do
    # Increment the total test count
    TOTAL_COUNT=$((TOTAL_COUNT + 1))

    # Use a simpler scheme for filenames
    SAFE_PARAM="test_${COUNTER}"
    COUNTER=$((COUNTER + 1))

    # Run valgrind with the current command, redirecting output to a file
    valgrind --leak-check=full --track-fds=yes $PROGRAM <<EOF > "/tmp/valgrind_output_${SAFE_PARAM}.txt" 2>&1
$PARAM
exit
EOF

    # Check for memory leaks
    if ! grep "file descriptor" "/tmp/valgrind_output_${SAFE_PARAM}.txt" | awk '/file descriptor [34567]:/ {print; f=1} END {exit !f}' && \
       ! grep -q "Invalid read of size " "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       ! grep -q "Invalid write of size " "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       ! grep -q "invalid file descriptor" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       ! grep -q "Warning: invalid file descriptor" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "definitely lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "indirectly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "possibly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt"; then
        echo "✅ Test ${COUNTER}: $PARAM"
        # Increment the success test count
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    else
        echo "❌ Test ${COUNTER}: $PARAM"
        # Uncomment the following lines to print debugging output
        echo "Debugging output for Test ${COUNTER}:"
        grep "file descriptor" "/tmp/valgrind_output_${SAFE_PARAM}.txt" | awk '/file descriptor [34567]:/'
        grep "invalid file descriptor" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
        grep "Invalid read of size" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
        grep "Invalid write of size" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
        grep "definitely lost:" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
        grep "indirectly lost:" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
        grep "possibly lost:" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
    fi
done

# Check if all tests passed
if [ "$SUCCESS_COUNT" -eq "$TOTAL_COUNT" ]; then
    # If all tests passed, print the result in green
    echo -e "${GREEN}Final Result: ${SUCCESS_COUNT}/${TOTAL_COUNT}${NO_COLOR}"
else
    # If any test failed, print the result in red
    echo -e "${RED}Final Result: ${SUCCESS_COUNT}/${TOTAL_COUNT}${NO_COLOR}"
fi

