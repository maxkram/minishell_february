#!/bin/bash

PROGRAM="./minishell"
# Define your test commands as before
INPUT_PARAMS=(

    # echo
    "echo hello world"
    "echo \"hello world\""
    "echo 'hello world'"
    "echo hello'world'"
    "echo hello\"\"world" # ❌
    "echo ''"
    "echo \"\$PWD\""
    "echo '\$PWD'"
    "echo \"aspas ->'\""
    "echo \"aspas -> ' \""
    "echo 'aspas ->\"'"
    "echo 'aspas -> \" '"
    "echo \"> >> < * ? [ ] | ; [ ] || && ( ) & # \$  <<\""
    "echo '> >> < * ? [ ] | ; [ ] || && ( ) & # \$  <<'"
    "echo \"exit_code ->\$? user ->\$USER home -> \$HOME\""
    "echo 'exit_code ->\$? user ->\$USER home -> \$HOME'"
    "echo \$"
    "echo \$?"
    "echo \$?HELLO"
    "echo Hello, World!"  # Basic command
    "echo 'Hello, World!'"  # Single quotes
    "echo \"Hello, World!\""  # Double quotes
    "echo Hello\, World\!"  # Escape characters
    "echo 'Hello, '\"World!\""  # Mixed quotes
    "echo \$HOME"  # Variable expansion
    "echo \$((2 + 2))"  # Arithmetic expansion
    "echo $(echo Nested)"  # Command substitution
    "echo `echo Backticks`"  # Command substitution with backticks
    "echo; echo Separate"  # Semicolon separator
    "echo 'Single quote -> '\"'\"' <- Single quote'"  # Single quotes inside double quotes
    "echo \"Double quote -> '\"'\"' <- Double quote\""  # Double quotes inside single quotes
    "echo Text > /tmp/output.txt"  # Redirection
    "cat < /tmp/output.txt"  # Input redirection
    "echo First && echo Second"  # AND list
    "echo First || echo Failed"  # OR list
    "echo \"Nested -> \$(echo command)\""  # Nested command substitution
    "echo 'A space -> '\"\ \""  # Escaped space
    "echo \"A new\nline\""  # Escaped new line
    "echo -e \"Tab\tcharacter\""  # Tab character with -e option
    "echo \"A quote ' inside a quote\""  # Quote inside quote
    "echo 'A quote \" inside a quote'"  # Quote inside quote reversed

    # exit
    "exit"
    "exit 0"
    "exit 1"
    "exit 255"
    "exit 256" # Depending on your shell, this might wrap around or be treated as an error
    "exit -1"
    "exit 42"
    "exit +42"
    "exit 9999999999" # Large number, potentially out-of-range
    "exit -9999999999" # Large negative number
    "exit abc" # Non-numeric argument
    "exit 42abc" # Numeric followed by characters
    "exit 42 58" # Multiple arguments, which should be invalid for exit

    # cd
    "cd /"  # Change to the root directory
    "cd /tmp"  # Change to the /tmp directory
    "cd /var/log"  # Change to the /var/log directory
    "cd .."  # Move up one directory
    "cd ../.."  # Move up two directories
    "cd ./"  # Stay in the current directory
    "cd -"  # Switch to the previous directory
    "cd \$HOME"  # Change to the user's home directory
    "cd \$PWD"  # Change to the current directory using PWD variable
    "cd \"Directory With Spaces\""  # Quotes around directory name with spaces
    "cd Directory\\ With\\ Spaces"  # Escape spaces within directory name
    "cd ./subdir"  # Change to a subdirectory
    "cd subdir/anotherdir"  # Change to a nested directory via relative path
    "cd nonexistentdir"  # Attempt to change to a directory that does not exist
    "cd /root"  # Attempt to change to a directory where the user likely lacks permissions
    "cd"  # Without any arguments, should change to the home directory
    "cd ~"  # Change to the home directory using tilde
    "cd ~/subdir"  # Change to a subdirectory within the home directory
    "cd ///"  # Multiple slashes, should resolve to root
    "cd ..//subdir"  # Use of multiple slashes in a relative path

    # env
    "env"  # Display all environment variables
    "env | grep PATH"  # Display the PATH environment variable
    "env | grep HOME"  # Display the HOME environment variable
    "env -i"  # Run env with an empty environment
    "env -i ls"  # Run a command (ls) with an empty environment
    "env | sort"  # Display all environment variables sorted alphabetically
    "env | wc -l"  # Count the number of environment variables
    "env -i env"  # Display environment variables after resetting to empty
    "env VAR1=test env"  # Set VAR1 and display environment variables
    "env VAR1=test VAR2=another_test env"  # Set multiple variables and display environment
    "env -i VAR1=test ls"  # Run a command with a single environment variable set
    "env | grep VAR1"  # Check if VAR1 is set in the environment

    # unset
    "export TEST_VAR1=hello && unset TEST_VAR1 && env | grep TEST_VAR1"  # Unset a variable and check it's gone
    "export TEST_VAR2=world && unset TEST_VAR2 && echo \$TEST_VAR2"  # Unset a variable and attempt to echo its value
    "export TEST_VAR3=testing && unset TEST_VAR3 TEST_VAR4 && env | grep TEST_VAR3"  # Unset multiple variables, check first is gone
    "export TEST_VAR4=anotherTest && unset TEST_VAR3 TEST_VAR4 && env | grep TEST_VAR4"  # Unset multiple variables, check second is gone
    "unset PATH && echo \$PATH"  # Unset PATH and check it's gone
    "export TEST_VAR5=foo && export TEST_VAR6=bar && unset TEST_VAR5 TEST_VAR6 && env | grep TEST_VAR5"  # Unset two variables, check first is gone
    "export TEST_VAR5=foo && export TEST_VAR6=bar && unset TEST_VAR5 TEST_VAR6 && env | grep TEST_VAR6"  # Unset two variables, check second is gone
    "unset NOT_SET_VAR && echo 'Unset a variable that was never set'"  # Attempt to unset a variable that doesn't exist

    # export
    "export NEW_VAR=new_value"  # Set a new environment variable
    "export NEW_VAR=new_value| echo \$NEW_VAR"  # Set and then print a new environment variable
    "export PATH=updated_value"  # Update an existing environment variable
    "export PATH= | echo \$EXISTING_VAR"  # Unset an existing environment variable's value
    "export"  # Display all exported environment variables
    "export VAR_WITH_SPACES='value with spaces'| echo \$VAR_WITH_SPACES"  # Set a variable with spaces in its value
    "export NUM_VAR=123| echo \$NUM_VAR"  # Set a numeric environment variable
    "export VAR_WITH_SPECIAL_CHARS='!@#\$%^&*()'| echo \$VAR_WITH_SPECIAL_CHARS"  # Set a variable with special characters
    "export PATH=\$PATH:/new/path"  # Append to the PATH variable
    "export -p"  # Display all exported variables (using -p option for portability)
    "export VAR_TO_UNSET=to_be_unset| unset VAR_TO_UNSET| echo \$VAR_TO_UNSET"  # Unset a variable and confirm it's gone
    "export VAR_WITH_EQUALS='equals==in_value'| echo \$VAR_WITH_EQUALS"  # Set a variable containing equals signs in its value
    "export 'VAR_WITH_QUOTES=\"quoted_value\"'| echo \$VAR_WITH_QUOTES"  # Set a variable with quoted value
    "export VAR_TO_BE_EXPORTED=value| env | grep VAR_TO_BE_EXPORTED"  # Export a variable and check it's in env
    "export hello"
    "export HELLO=123"
    "export A-"
    "export HELLO=123 A"
    "export HELLO=\"123 A-\""
    "export hello world"
    "export HELLO-=123"
    "export ="
    "export 123"


    # pwd
    "pwd /"
    "pwd ~"
    "pwd .."
    "pwd ../../"
    "pwd /usr/bin"
    "pwd /etc/"
    "pwd usr/local/share"

    # ls
    "ls >./outfiles/outfile01"
    "ls > ./outfiles/outfile01"
    "echo hi > ./outfiles/outfile01 bye"
    "ls >./outfiles/outfile01 >./outfiles/outfile02"
    "ls >./outfiles/outfile01 >./test_files/invalid_permission"
    "ls >\"./outfiles/outfile with spaces\""
    "ls >\"./outfiles/outfile\"\"1\"\"2\"\"3\"\"4\"\"5\""
    "ls >\"./outfiles/outfile01\" >./test_files/invalid_permission >\"./outfiles/outfile02\""
    "ls >./test_files/invalid_permission >\"./outfiles/outfile01\" >./test_files/invalid_permission"


    # Variables expansion
    "echo \$HOME"  # Simple variable expansion
    "echo \$PATH"  # Expand PATH variable
    "echo \$USER"  # Expand USER variable
    "export TEST_VAR='Hello World' | echo \$TEST_VAR"  # Set and expand custom variable export TEST_VAR='Hello World'; echo $TEST_VAR
    "echo \${UNSET_VAR:-default}"  # Use default value for unset variable echo ${UNSET_VAR:-default}
    "echo \${HOME:-/home/default}"  # Use default value for set variable (should print HOME value) echo ${HOME:-/home/default}
    "export VAR='value' | echo \${VAR:+alt_value}"  # Use alternative value if VAR is set export VAR='value'; echo ${VAR:+alt_value}
    "echo \$(date)"  # Command substitution echo $(date)
    "echo \$(echo \${HOME})"  # Nested command substitution echo $(echo ${HOME})
    "echo \$((2 + 2))"  # Arithmetic expansion echo $((2 + 2))
    "echo \$((\$RANDOM % 5))"  # Arithmetic expansion with variable echo $((RANDOM % 5))
    "export EMPTY_VAR='' | echo \${EMPTY_VAR:-'Default Value'}"  # Default value for empty variable export EMPTY_VAR=''; echo ${EMPTY_VAR:-'Default Value'}
    "export SET_VAR='Set' | unset SET_VAR | echo \${SET_VAR:-'Was unset'}"  # Check unset variable export SET_VAR='Set'; unset SET_VAR; echo ${SET_VAR:-'Was unset'}
    "echo \"Nested \${VAR:\${1:\${2:1}}}\""  # Nested braces (invalid, for error handling) export VAR='world'; echo "Nested ${VAR:${1:${2:1}}}"
    "echo \${#HOME}"  # Length of variable
    "echo \${HOME:0:4}"  # Substring of variable echo ${HOME:0:4}
    "export VAR='Hello' | echo \${VAR^}"  # Uppercase first letter export VAR='hello'; echo ${VAR^}
    "export VAR='hello' | echo \${VAR^^}"  # Uppercase all letters export VAR='hello'; echo ${VAR^^}
    "export VAR='HELLO' | echo \${VAR,}"  # Lowercase first letter export VAR='HELLO'; echo ${VAR,}
    "export VAR='HELLO' | echo \${VAR,,}"  # Lowercase all letters export VAR='HELLO'; echo ${VAR,,}
    "export PATH_VAR='/usr/local/bin:/usr/bin:/bin' | echo \${PATH_VAR//:/\n}"  # Replace all colons with newlines export PATH_VAR='/usr/local/bin:/usr/bin:/bin'; echo ${PATH_VAR//:/\n}
    "export MISSING_VAR | echo \${MISSING_VAR?'Error: Variable is unset.'}"  # Error if variable is unset export MISSING_VAR; echo ${MISSING_VAR?'Error: Variable is unset.'}


    # Pipelines and Redirections
    "echo 'Hello World' | wc -c"  # Simple pipeline
    "cat /etc/passwd | grep root | wc -l"  # Pipeline with three commands
    "echo 'Hello from shell' > /tmp/testfile.txt"  # Output redirection
    "cat < /tmp/testfile.txt"  # Input redirection
    "grep shell < /tmp/testfile.txt | sort > /tmp/sorted.txt"  # Combine input and output redirection with pipeline
    "cat /etc/passwd | grep nologin | wc -l > /tmp/nologin_count.txt"  # Pipeline output redirection
    "echo 'case insensitive' | tr '[:lower:]' '[:upper:]'"  # Use of tr command in pipeline
    "cat /etc/passwd | awk -F: '{print \$1}' | sort | uniq"  # Complex pipeline with awk
    "cat /dev/null > /tmp/emptyfile.txt"  # Truncate file
    "echo 'append this text' >> /tmp/testfile.txt"  # Append redirection
    "cat /etc/passwd | head -n 5"  # Pipeline with head command
    "cat /etc/passwd | tail -n 5"  # Pipeline with tail command
    "cat /etc/hosts | grep localhost | tee /tmp/localhost.txt"  # Use of tee for output branching
    "cat < /tmp/testfile.txt | grep 'Hello' | sort -r > /tmp/sorted_greets.txt"  # Complex pipeline combining input and output redirection
    "cat /nonexistentfile 2> /tmp/stderr.txt"  # Redirect stderr to a file
    "cat access.log | awk '{print $1}' | sort | uniq -c | sort -nr | awk '{print $2, "requests:", $1}'"
    "find . -type f | rev | cut -d. -f1 | rev | sort | uniq -c | sort -nr | awk '{print $2, "files:", $1}'"
    "ps aux | sort -nk +4 | tail | awk '{print $11, "memory usage:", $4"%"}'"
    "git log --pretty=format:%aN | sort | uniq -c | sort -nr | awk '{print $2, "lines changed:", $1}'"
    "find . -type f | xargs -L1 dirname | sort | uniq -c | sort -nr | awk '{print $2, "files:", $1}'"
    "netstat -ntu | awk '{print $5}' | cut -d: -f1 | sort | uniq -c | sort -nr | awk '{print $2, "connections:", $1}'"
    "grep -rI "pattern" . | wc -l | sort -nr | awk '{print $2, "occurrences:", $1}'"
    "grep -rI "pattern" . | cut -d: -f1 | uniq -c | sort -nr | awk '{print $2, "occurrences:", $1}'"


    # File operations
    "grep hi <./test_files/infile"
    "grep hi \"<infile\" < ./test_files/infile"
    "echo hi < ./test_files/infile bye bye"
    "grep hi <./test_files/infile_big <./test_files/infile"
    "echo <\"./test_files/infile\" \"bonjour       42\""
    "cat <\"./test_files/file name with spaces\""
    "cat <./test_files/infile_big ./test_files/infile"
    "cat <\"1\"\"2\"\"3\"\"4\"\"5\""
    "echo <\"./test_files/infile\" <missing <\"./test_files/infile\""
    "echo <missing <\"./test_files/infile\" <missing"
    "cat <\"./test_files/infile\""
    "echo <\"./test_files/infile_big\" | cat <\"./test_files/infile\""
    "echo <\"./test_files/infile_big\" | cat \"./test_files/infile\""
    "echo <\"./test_files/infile_big\" | echo <\"./test_files/infile\""
    "echo hi | cat <\"./test_files/infile\""
    "echo hi | cat \"./test_files/infile\""
    "cat <\"./test_files/infile\" | echo hi"
    "cat <\"./test_files/infile\" | grep hello"
    "cat <\"./test_files/infile_big\" | echo hi"
    "cat <missing"
    "cat <missing | cat"
    "cat <missing | echo oi"
    "cat <missing | cat <\"./test_files/infile\""
    "echo <123 <456 hi | echo 42"

    # Heredoc
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

# Use a counter to create unique filenames
COUNTER=0

for PARAM in "${INPUT_PARAMS[@]}"; do
    # Use a simpler scheme for filenames
    SAFE_PARAM="test_${COUNTER}"
    COUNTER=$((COUNTER + 1))

    # Run valgrind with the current command, redirecting output to a file
    valgrind --leak-check=full $PROGRAM <<EOF > "/tmp/valgrind_output_${SAFE_PARAM}.txt" 2>&1
$PARAM
exit
EOF

    # Check for memory leaks
    if grep -q "definitely lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -vq "Invalid read of size " "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "indirectly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "possibly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt"; then
        echo "✅ Test ${COUNTER}: $PARAM"
    else
        echo "❌ Test ${COUNTER}: $PARAM"
        # Uncomment the following lines to print debugging output
        echo "Debugging output for Test ${COUNTER}:"
        grep "definitely lost:" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
        grep "Invalid read of size" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
        grep "indirectly lost:" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
        grep "possibly lost:" "/tmp/valgrind_output_${SAFE_PARAM}.txt"
    fi
done
