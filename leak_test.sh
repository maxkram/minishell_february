#!/bin/bash

# Define ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
NO_COLOR='\033[0m' # No Color


PROGRAM="./minishell"
# Define your test commands as before
INPUT_PARAMS=(

    # # Open file descriptors
    # "echo <"./test_files/infile_big" | echo <"./test_files/infile"" # no leaks
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
    "echo hello\"\"world" # Test 5
    "echo ''" # Test 6
    "echo \"\$PWD\"" # Test 7
    "echo '\$PWD'" # Test 8
    "echo \"aspas ->'\"" # Test 9
    "echo \"aspas -> ' \"" # Test 10
    "echo 'aspas ->\"'" # Test 11
    "echo 'aspas -> \" '" # Test 12
    "echo \"> >> < * ? [ ] | ; [ ] || && ( ) & # \$  <<\"" # Test 13
    "echo '> >> < * ? [ ] | ; [ ] || && ( ) & # \$  <<'" # Test 14
    "echo \"exit_code ->\$? user ->\$USER home -> \$HOME\"" # Test 15
    "echo 'exit_code ->\$? user ->\$USER home -> \$HOME'" # Test 16
    "echo \$" # Test 17
    "echo \$?" # Test 18
    "echo \$?HELLO" # Test 19
    "echo Hello, World!" # Test 20 (no quotes)
    "echo 'Hello, World!'" # Test 21 (single quotes)
    "echo \"Hello, World!\""  # Test 22 (double quotes)
    "echo Hello\, World\!"  # Test 23 (escaped characters)
    "echo 'Hello, '\"World!\"" # Test 24 (single quotes with escaped double quotes)
    "echo \$HOME" # Test 25 (variable expansion)
    "echo \$((2 + 2))"  # Test 26 (arithmetic expansion)
    "echo $(echo Nested)"  # Test 27 (command substitution)
    "echo `echo Backticks`"  # Test 28 (backticks)
    "echo; echo Separate"  # Test 29 (multiple commands)
    "echo 'Single quote -> '\"'\"' <- Single quote'" # Test 30 (single quotes inside single quotes)
    "echo \"Double quote -> '\"'\"' <- Double quote\"" # Test 31 (single quotes inside double quotes)
    "echo Text > /tmp/output.txt" # Test 32 (output redirection)
    "cat < /tmp/output.txt" # Test 33 (input redirection)
    "echo First && echo Second" # Test 34 (AND list)
    "echo First || echo Failed" # Test 35 (OR list)
    "echo \"Nested -> \$(echo command)\"" # Test 36 (nested command substitution)
    "echo 'A space -> '\"\ \"" # Test 37 (space inside quotes)
    "echo \"A new\nline\"" # Test 38 (newline inside double quotes)
    "echo -e \"Tab\tcharacter\"" # Test 39 (tab character)
    "echo \"A quote ' inside a quote\"" # Test 40 (quote inside double quotes)
    "echo 'A quote \" inside a quote'" # Test 41 (quote inside single quotes)

    # exit
    "exit" # Test 42 (no arguments)
    "exit 0" # Test 43 (numeric argument)
    "exit 1" # Test 44 (numeric argument)
    "exit 255" # Test 45 (numeric argument)
    "exit 256" # Test 46 (numeric argument)
    "exit -1" # Test 47 (negative argument)
    "exit 42" # Test 48 (numeric argument)
    "exit +42" # Test 49 (positive argument)
    "exit 9999999999" # Test 50 (large positive number)
    "exit -9999999999" # Test 51 (large negative number)
    "exit abc" # Test 52 (non-numeric argument)
    "exit 42abc" # Test 53 (non-numeric argument)
    "exit 42 58" # Test 54 (too many arguments)

    # cd
    "cd /"  # Test 55 (root directory)
    "cd /tmp"  # Test 56 (existing directory)
    "cd /var/log"  # Test 57 (existing directory)
    "cd .."  # Test 58 (parent directory)
    "cd ../.."  # Test 59 (grandparent directory)
    "cd ./"  # Test 60 (current directory)
    "cd -"  # Test 61 (previous directory)
    "cd \$HOME"  # Test 62 (home directory)
    "cd \$PWD"  # Test 63 (current directory)
    "cd \"Directory With Spaces\""  # Test 64 (directory with spaces)
    "cd Directory\\ With\\ Spaces"  # Test 65 (directory with spaces)
    "cd ./subdir"  # Test 66 (subdirectory)
    "cd subdir/anotherdir"  # Test 67 (subdirectory)
    "cd nonexistentdir"  # Test 68 (nonexistent directory)
    "cd /root"  # Test 69 (directory with restricted access)
    "cd"  # Test 70 (no arguments)
    "cd ~"  # Test 71 (home directory)
    "cd ~/subdir"  # Test 72 (subdirectory)
    "cd ///"  # Test 73 (redundant slashes)
    "cd ..//subdir"  # Test 74 (redundant slashes)

    # env
    "env"  # Test 75 (no arguments)
    "env | grep PATH"  # Test 76 (display PATH variable)
    "env | grep HOME"  # Test 77 (display HOME variable)
    "env -i"  # Test 78 (reset environment)
    "env -i ls"  # Test 79 (reset environment and run command)
    "env | sort"  # Test 80 (sort environment variables)
    "env | wc -l"  # Test 81 (count environment variables)
    "env -i env" # Test 82 (reset environment and display environment variables)
    "env VAR1=test env"  # Test 83 (set variable and display environment)
    "env VAR1=test VAR2=another_test env" # Test 84 (set multiple variables and display environment)
    "env -i VAR1=test ls"  # Test 85 (reset environment and set variable for command)
    "env | grep VAR1"  # Test 86 (display variable set in previous command)

    # unset
    "export TEST_VAR1=hello && unset TEST_VAR1 && env | grep TEST_VAR1"  # Test 87 (unset variable and check it's gone)
    "export TEST_VAR2=world && unset TEST_VAR2 && echo \$TEST_VAR2"  # Test 88 (unset variable and check it's gone)
    "export TEST_VAR3=testing && unset TEST_VAR3 TEST_VAR4 && env | grep TEST_VAR3"  # Test 89 (unset multiple variables, check first is gone)
    "export TEST_VAR4=anotherTest && unset TEST_VAR3 TEST_VAR4 && env | grep TEST_VAR4" # Test 90 (unset multiple variables, check second is gone)
    "unset PATH && echo \$PATH" # Test 91 (unset PATH variable and check it's gone)
    "export TEST_VAR5=foo && export TEST_VAR6=bar && unset TEST_VAR5 TEST_VAR6 && env | grep TEST_VAR5" # Test 92 (unset two variables, check first is gone)
    "export TEST_VAR5=foo && export TEST_VAR6=bar && unset TEST_VAR5 TEST_VAR6 && env | grep TEST_VAR6" # Test 93 (unset two variables, check second is gone)
    "unset NOT_SET_VAR && echo 'Unset a variable that was never set'" # Test 94 (unset variable that was never set)

    # export
    "export NEW_VAR=new_value" # Test 95 (set a new environment variable)
    "export NEW_VAR=new_value| echo \$NEW_VAR" # Test 96 (set a new environment variable and check it's set)
    "export PATH=updated_value" # Test 97 (update an existing environment variable's value)
    "export PATH= | echo \$EXISTING_VAR" # Test 98 (update an existing environment variable's value to empty)
    "export" # Test 99 (display all exported variables)
    "export VAR_WITH_SPACES='value with spaces'| echo \$VAR_WITH_SPACES" # Test 100 (set a variable with spaces)
    "export NUM_VAR=123| echo \$NUM_VAR" # Test 101 (set a variable with a number)
    "export VAR_WITH_SPECIAL_CHARS='!@#\$%^&*()'| echo \$VAR_WITH_SPECIAL_CHARS" # Test 102 (set a variable with special characters)
    "export PATH=\$PATH:/new/path" # Test 103 (append to an existing environment variable's value)
    "export -p" # Test 104 (display all exported variables in a format that can be reused as input)
    "export VAR_TO_UNSET=to_be_unset| unset VAR_TO_UNSET| echo \$VAR_TO_UNSET" # Test 105 (unset a variable and check it's gone)
    "export VAR_WITH_EQUALS='equals==in_value'| echo \$VAR_WITH_EQUALS" # Test 106 (set a variable with equals in value)
    "export 'VAR_WITH_QUOTES=\"quoted_value\"'| echo \$VAR_WITH_QUOTES" # Test 107 (set a variable with quotes in value)
    "export VAR_TO_BE_EXPORTED=value| env | grep VAR_TO_BE_EXPORTED" # Test 108 (export a variable and check it's in the environment)
    "export hello" # Test 109 (export a variable without value)
    "export HELLO=123" # Test 110 (export a variable with a number)
    "export A-" # Test 111 (export a variable with a special character)
    "export HELLO=123 A" # Test 112 (export a variable with a space)
    "export HELLO=\"123 A-\"" # Test 113 (export a variable with a space and special character)
    "export hello world" # Test 114 (export two variables without value)
    "export HELLO-=123" # Test 115 (export a variable with a special character)
    "export =" # Test 116 (export a variable with a special character)
    "export 123" # Test 117 (export a variable with a number)


    # pwd
    "pwd /" # Test 118 (root directory)
    "pwd ~" # Test 119 (home directory)
    "pwd .." # Test 120 (parent directory)
    "pwd ../../" # Test 121 (grandparent directory)
    "pwd /usr/bin" # Test 122 (existing directory)
    "pwd /etc/" # Test 123 (existing directory with trailing slash)
    "pwd usr/local/share" # Test 124 (nonexistent directory)

    # ls
    "ls >./outfiles/outfile01" # Test 125 (output redirection)
    "ls > ./outfiles/outfile01" # Test 126 (output redirection with space)
    "echo hi > ./outfiles/outfile01 bye" # Test 127 (output redirection with extra arguments)
    "ls >./outfiles/outfile01 >./outfiles/outfile02" # Test 128 (multiple output redirections)
    "ls >./outfiles/outfile01 >./test_files/invalid_permission" # Test 129 (output redirection with invalid permission)
    "ls >\"./outfiles/outfile with spaces\"" # Test 130 (output redirection with spaces)
    "ls >\"./outfiles/outfile\"\"1\"\"2\"\"3\"\"4\"\"5\"" # Test 131 (output redirection with special characters)
    "ls >\"./outfiles/outfile01\" >./test_files/invalid_permission >\"./outfiles/outfile02\"" # Test 132 (multiple output redirections with invalid permission)
    "ls >./test_files/invalid_permission >\"./outfiles/outfile01\" >./test_files/invalid_permission" # Test 133 (multiple output redirections with invalid permission)


    # Variables expansion
    "echo \$HOME" # Test 134 (expand HOME variable)
    "echo \$PATH" # Test 135 (expand PATH variable)
    "echo \$USER" # Test 136 (expand USER variable)
    "export TEST_VAR='Hello World' | echo \$TEST_VAR" # Test 137 (expand custom variable)
    "echo \${UNSET_VAR:-default}" # Test 138 (expand unset variable with default value)
    "echo \${HOME:-/home/default}" # Test 139 (expand set variable with default value)
    "export VAR='value' | echo \${VAR:+alt_value}" # Test 140 (expand set variable with alternative value)
    "echo \$(date)" # Test 141 (expand command substitution)
    "echo \$(echo \${HOME})" # Test 142 (expand nested variable)
    "echo \$((2 + 2))" # Test 143 (expand arithmetic expansion)
    "echo \$((\$RANDOM % 5))" # Test 144 (expand arithmetic expansion with variable)
    "export EMPTY_VAR='' | echo \${EMPTY_VAR:-'Default Value'}" # Test 145 (expand empty variable with default value)
    "export SET_VAR='Set' | unset SET_VAR | echo \${SET_VAR:-'Was unset'}" # Test 146 (expand unset variable with default value)
    "echo \"Nested \${VAR:\${1:\${2:1}}}\"" # Test 147 (expand nested variable with nested substring)
    "echo \${#HOME}" # Test 148 (expand length of variable)
    "echo \${HOME:0:4}" # Test 149 (expand substring of variable)
    "export VAR='Hello' | echo \${VAR^}" # Test 150 (expand uppercase first letter)
    "export VAR='hello' | echo \${VAR^^}" # Test 151 (expand uppercase all letters)
    "export VAR='HELLO' | echo \${VAR,}" # Test 152 (expand lowercase first letter)
    "export VAR='HELLO' | echo \${VAR,,}" # Test 153 (expand lowercase all letters)
    "export PATH_VAR='/usr/local/bin:/usr/bin:/bin' | echo \${PATH_VAR//:/\n}" # Test 154 (expand replace all)
    "export MISSING_VAR | echo \${MISSING_VAR?'Error: Variable is unset.'}" # Test 155 (expand error if unset)


    # Pipelines and Redirections
    "echo 'Hello World' | wc -c" # Test 156 (pipeline with wc)
    "cat /etc/passwd | grep root | wc -l" # Test 157 (pipeline with grep and wc)
    "echo 'Hello from shell' > /tmp/testfile.txt" # Test 158 (output redirection)
    "cat < /tmp/testfile.txt" # Test 159 (input redirection)
    "grep shell < /tmp/testfile.txt | sort > /tmp/sorted.txt" # Test 160 (pipeline with input and output redirection)
    "cat /etc/passwd | grep nologin | wc -l > /tmp/nologin_count.txt" # Test 161 (pipeline with wc and output redirection)
    "echo 'case insensitive' | tr '[:lower:]' '[:upper:]'" # Test 162 (pipeline with tr)
    "cat /etc/passwd | awk -F: '{print \$1}' | sort | uniq" # Test 163 (pipeline with awk, sort, and uniq)
    "cat /dev/null > /tmp/emptyfile.txt" # Test 164 (output redirection to empty file)
    "echo 'append this text' >> /tmp/testfile.txt" # Test 165 (append output redirection)
    "cat /etc/passwd | head -n 5" # Test 166 (pipeline with head)
    "cat /etc/passwd | tail -n 5" # Test 167 (pipeline with tail)
    "cat /etc/hosts | grep localhost | tee /tmp/localhost.txt" # Test 168 (pipeline with tee)
    "cat < /tmp/testfile.txt | grep 'Hello' | sort -r > /tmp/sorted_greets.txt" # Test 169 (input redirection with grep and output redirection with sort)
    "cat /nonexistentfile 2> /tmp/stderr.txt" # Test 170 (output redirection of stderr)
    "cat access.log | awk '{print $1}' | sort | uniq -c | sort -nr | awk '{print $2, "requests:", $1}'" # Test 171 (pipeline with multiple commands)
    "find . -type f | rev | cut -d. -f1 | rev | sort | uniq -c | sort -nr | awk '{print $2, "files:", $1}'" # Test 172 (pipeline with multiple commands)
    "ps aux | sort -nk +4 | tail | awk '{print $11, memory usage:, $4"%"}'" # Test 173 (pipeline with multiple commands)
    "git log --pretty=format:%aN | sort | uniq -c | sort -nr | awk '{print $2, lines changed:, $1}'" # Test 174 (pipeline with multiple commands)
    "find . -type f | xargs -L1 dirname | sort | uniq -c | sort -nr | awk '{print $2, "files:", $1}'" # Test 175 (pipeline with multiple commands)
    "netstat -ntu | awk '{print $5}' | cut -d: -f1 | sort | uniq -c | sort -nr | awk '{print $2, "connections:", $1}'" # Test 176 (pipeline with multiple commands)
    "grep -rI "pattern" . | wc -l | sort -nr | awk '{print $2, "occurrences:", $1}'" # Test 177 (pipeline with multiple commands)
    "grep -rI "pattern" . | cut -d: -f1 | uniq -c | sort -nr | awk '{print $2, "occurrences:", $1}'" # Test 178 (pipeline with multiple commands)


    # File operations
    "grep hi <./test_files/infile" # Test 179 (input redirection with grep)
    "grep hi \"<infile\" < ./test_files/infile" # Test 180 (input redirection with grep and quotes)
    "echo hi < ./test_files/infile bye bye" # Test 181 (input redirection with extra arguments)
    "grep hi <./test_files/infile_big <./test_files/infile" # Test 182 (input redirection with grep and large file)
    "echo <\"./test_files/infile\" \"bonjour       42\"" # Test 183 (input redirection with quotes and spaces)
    "cat <\"./test_files/file name with spaces\"" # Test 184 (input redirection with file with spaces)
    "cat <./test_files/infile_big ./test_files/infile" # Test 185 (input redirection with multiple files)
    "cat <\"1\"\"2\"\"3\"\"4\"\"5\"" # Test 186 (input redirection with special characters)
    "echo <\"./test_files/infile\" <missing <\"./test_files/infile\"" # Test 187 (input redirection with missing file)
    "echo <missing <\"./test_files/infile\" <missing" # Test 188 (input redirection with multiple missing files)
    "cat <\"./test_files/infile\"" # Test 189 (input redirection with cat)
    "echo <\"./test_files/infile_big\" | cat <\"./test_files/infile\"" # Test 190 (input redirection with cat and large file)
    "echo <\"./test_files/infile_big\" | cat \"./test_files/infile\"" # Test 191 (input redirection with cat and large file)
    "echo <\"./test_files/infile_big\" | echo <\"./test_files/infile\"" # Test 192 (input redirection with echo and large file)
    "echo hi | cat <\"./test_files/infile\"" # Test 193 (input redirection with echo and cat)
    "echo hi | cat \"./test_files/infile\"" # Test 194 (input redirection with echo and cat)
    "cat <\"./test_files/infile\" | echo hi" # Test 195 (input redirection with cat and echo)
    "cat <\"./test_files/infile\" | grep hello" # Test 196 (input redirection with cat and grep)
    "cat <\"./test_files/infile_big\" | echo hi" # Test 197 (input redirection with cat and echo)
    "cat <missing" # Test 198 (input redirection with missing file)
    "cat <missing | cat" # Test 199 (input redirection with missing file and cat)
    "cat <missing | echo oi" # Test 200 (input redirection with missing file and echo)
    "cat <missing | cat <\"./test_files/infile\"" # Test 201 (input redirection with missing file and cat)
    "echo <123 <456 hi | echo 42" # Test 202 (input redirection with numbers and echo)

    # Heredoc
    "cat '<<' EOF" # Test 203 (heredoc with missing delimiter)
    "export VAR='World'; cat <<EOF\nHello, \$VAR!\nEOF" # Test 204 (heredoc with variable expansion)
    "cat <<EOF\nCurrent date: \$(date)\nEOF" # Test 205 (heredoc with command substitution)
    "cat <<EOF1\nThis is the outer heredoc.\n$(cat <<EOF2\nThis is the inner heredoc.\nEOF2\n)\nEOF1" # Test 206 (nested heredoc)
    "cat <<EOF\nThis is a test with an escaped variable: \\\$VAR\nEOF" # Test 207 (heredoc with escaped variable)
    "cat <<EOF\nThis is a test without a trailing newlineEOF" # Test 208 (heredoc without trailing newline)
    "cat <<EOF > /tmp/heredoc_test.txt\nThis content will be written to a file.\nEOF" # Test 209 (heredoc with redirection)
    "cat <<EOF | grep 'test'\nThis is a test line.\nThis line does not match.\nEOF" # Test 210 (heredoc with pipeline)
    "export VAR='World'; cat <<'EOF'\nHello, \$VAR!\nEOF" # Test 211 (heredoc with single quotes)
    "cat <<EOF > /tmp/heredoc_complex_test.txt\nThis file was generated on: \$(date)\nEOF" # Test 212 (heredoc with redirection and command substitution)
    "while read line; do echo \"Line: \$line\"; done <<EOF\nFirst line\nSecond line\nEOF" # Test 213 (heredoc with while loop)

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
