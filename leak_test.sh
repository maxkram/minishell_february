# #!/bin/bash

# PROGRAM="./minishell"
# INPUT_PARAMS_BASIC=(
#     "ls"
#     "echo Hello"
#     "pwd"
#     "cat access.log | awk '{print \$1}' | sort | uniq -c | sort -nr | awk '{print \$2, \"requests:\", \$1}'"
#     "echo hello world"
#     "echo \"hello world\""
#     "echo 'hello world'"
#     "echo hello'world'"
#     "echo hello\"\"world"
#     "echo ''"
# )

# INPUT_PARAMS_ENV=(
#     "export MY_VAR=Hello"
#     "echo \"\$PWD\""
#     "echo '\$PWD'"
#     "echo \"exit_code ->\$? user ->\$USER home -> \$HOME\""
#     "echo 'exit_code ->\$? user ->\$USER home -> \$HOME'"
#     "export hello"
#     "export HELLO=123"
#     "export A-"
#     "export HELLO=123 A"
#     "export HELLO=\"123 A-\""
#     "export hello world"
#     "export HELLO-=123"
#     "export ="
#     "export 123"
#     "unset"
#     "unset HELLO"
#     "unset HELLO1 HELLO2"
#     "unset HOME"
#     "unset PATH"
#     "unset SHELL"
# )

# INPUT_PARAMS_FILE=(
#     "echo \"aspas ->'\""
#     "echo \"aspas -> ' \""
#     "echo 'aspas ->\"'"
#     "echo 'aspas -> \" '"
#     "echo \"> >> < * ? [ ] | ; [ ] || && ( ) & # \$  <<\""
#     "echo '> >> < * ? [ ] | ; [ ] || && ( ) & # \$  <<'"
# )

# INPUT_PARAMS_ERROR=(
#     "echo \$"
#     "echo \$?"
#     "echo \$?HELLO"
#     "pwd oi"
#     "cd \$PWD"
#     "cd \$PWD hi"
#     "cd 123123"
#     "exit 123"
#     "exit 298"
#     "exit +100"
#     "exit \"+100\""
#     "exit +\"100\""
#     "exit -100"
#     "exit \"-100\""
#     "exit -\"100\""
#     "exit hello"
#     "exit 42 world"
# )

# # INPUT_PARAMS_FILE_OPERATIONS=(
# #     "grep hi <./test_files/infile"
# #     "grep hi \"<infile\" < ./test_files/infile"
# #     "echo hi < ./test_files/infile bye bye"
# #     "grep hi <./test_files/infile_big <./test_files/infile"
# #     "echo <\"./test_files/infile\" \"bonjour       42\""
# #     "cat <\"./test_files/file name with spaces\""
# #     "cat <./test_files/infile_big ./test_files/infile"
# #     "cat <\"1\"\"2\"\"3\"\"4\"\"5\""
# #     "echo <\"./test_files/infile\" <missing <\"./test_files/infile\""
# #     "echo <missing <\"./test_files/infile\" <missing"
# #     "cat <\"./test_files/infile\""
# #     "echo <\"./test_files/infile_big\" | cat <\"./test_files/infile\""
# #     "echo <\"./test_files/infile_big\" | cat \"./test_files/infile\""
# #     "echo <\"./test_files/infile_big\" | echo <\"./test_files/infile\""
# #     "echo hi | cat <\"./test_files/infile\""
# #     "echo hi | cat \"./test_files/infile\""
# #     "cat <\"./test_files/infile\" | echo hi"
# #     "cat <\"./test_files/infile\" | grep hello"
# #     "cat <\"./test_files/infile_big\" | echo hi"
# #     "cat <missing"
# #     "cat <missing | cat"
# #     "cat <missing | echo oi"
# #     "cat <missing | cat <\"./test_files/infile\""
# #     "echo <123 <456 hi | echo 42"
# # )

# # INPUT_PARAMS_FILE_OPERATIONS_OUTPUT=(
# #     "ls >./outfiles/outfile01"
# #     "ls > ./outfiles/outfile01"
# #     "echo hi > ./outfiles/outfile01 bye"
# #     "ls >./outfiles/outfile01 >./outfiles/outfile02"
# #     "ls >./outfiles/outfile01 >./test_files/invalid_permission"
# #     "ls >\"./outfiles/outfile with spaces\""
# #     "ls >\"./outfiles/outfile\"\"1\"\"2\"\"3\"\"4\"\"5\""
# #     "ls >\"./outfiles/outfile01\" >./test_files/invalid_permission >\"./outfiles/outfile02\""
# #     "ls >./test_files/invalid_permission >\"./outfiles/outfile01\" >./test_files/invalid_permission"
# #     "cat <\"./test_files/infile\" >\"./outfiles/outfile01\""
# #     "echo hi >./outfiles/outfile01 | echo bye"
# #     "echo hi >./outfiles/outfile01 >./outfiles/outfile02 | echo bye"
# #     "echo hi | echo >./outfiles/outfile01 bye"
# #     "echo hi | echo bye >./outfiles/outfile01 >./outfiles/outfile02"
# #     "echo hi >./outfiles/outfile01 | echo bye >./outfiles/outfile02"
# #     "echo hi >./outfiles/outfile01 >./test_files/invalid_permission | echo bye"
# #     "echo hi >./test_files/invalid_permission | echo bye"
# #     "echo hi >./test_files/invalid_permission >./outfiles/outfile01 | echo bye"
# #     "echo hi | echo bye >./test_files/invalid_permission"
# #     "echo hi | >./outfiles/outfile0/tmp/valgrind_output_grep_hi_<./test_files/infile.txt1 echo bye >./test_files/invalid_permission"
# #     "echo hi | echo bye >./test_files/invalid_permission >./outfiles/outfile01"
# #     "cat <\"./test_files/infile\" >./test_files/invalid_permission"
# #     "cat >./test_files/invalid_permission <\"./test_files/infile\""
# #     "cat <missing >./outfiles/outfile01"
# #     "cat >./outfiles/outfile01 <missing"
# #     "cat <missing >./test_files/invalid_permission"
# #     "cat >./test_files/invalid_permission <missing"
# #     "cat >./outfiles/outfile01 <missing >./test_files/invalid_permission"
# # )

# # Add more tests as needed

# INPUT_PARAMS=(
#     "${INPUT_PARAMS_BASIC[@]}"
#     "${INPUT_PARAMS_ENV[@]}"
#     "${INPUT_PARAMS_FILE[@]}"
#     "${INPUT_PARAMS_ERROR[@]}"
#     "${INPUT_PARAMS_FILE_OPERATIONS[@]}"
#     "${INPUT_PARAMS_FILE_OPERATIONS_OUTPUT[@]}"
# )

# for PARAM in "${INPUT_PARAMS[@]}"; do
#     # Sanitize PARAM for filename by replacing spaces with underscores
#     SAFE_PARAM=$(echo "$PARAM" | sed 's/ /_/g')
#     # Run valgrind, redirecting output to a file with the sanitized PARAM in the filename
#     valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $PROGRAM <<EOF > "/tmp/valgrind_output_${SAFE_PARAM}.txt" 2>&1
# $PARAM
# exit
# EOF
#     # Simplified check for memory leaks
#     if grep -q "definitely lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
#        grep -q "indirectly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
#        grep -q "possibly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt"; then
#         echo "✅ $PARAM"
#     else
#         echo "❌ $PARAM"
#     fi
# done

# #!/bin/bash

# PROGRAM="./minishell"

# # INPUT_PARAMS_BASIC=(
# #     "ls"
# #     "echo Hello"
# #     "pwd"
# #     "cat access.log | awk '{print \$1}' | sort | uniq -c | sort -nr | awk '{print \$2, \"requests:\", \$1}'"
# #     "echo hello world"
# #     "echo "hello world""
# #     "echo 'hello world'"
# #     "echo hello'world'"
# #     "echo hello\"\"world"
# #     "echo ''"
# # )

# INPUT_PARAMS=(
#     "${INPUT_PARAMS_BASIC[@]}"

#     #echo
#     "echo hello world"
#     "echo \"hello world\""
#     "echo 'hello world'"
#     "echo hello'world'"
#     "echo hello\"\"world"
#     "echo hello""world"
#     "echo ''"
#     "echo "\"$PWD"\" "

#     # # exit
#     # "exit 0"
#     # "exit 1"
#     # "exit 255"
#     # "exit -1"
#     # "exit "custom message""
#     # "exit "more messages here""

#     # # cd
#     # "cd /"
#     # "cd ~"
#     # "cd .."
#     # "cd ../../"
#     # "cd /usr/bin"
#     # "cd /etc/"
#     # "cd /usr/local/share"

#     # # env
#     # "env"
#     # "env | grep PATH"
#     # "env | grep USER"
#     # "env | grep HOME"
#     # "env | grep SHELL"
#     # "env | grep LANG"
#     # "env | grep TERM"

#     # # unset
#     # "unset VAR_NAME"
#     # "unset ANOTHER_VAR"
#     # "unset MY_VAR"
#     # "unset HELLO_WORLD"

#     # # export
#     # "export VAR_NAME=value"
#     # "export MY_VAR=Hello"
#     # "export HELLO=World"
#     # "export PATH=/usr/bin:/bin"
#     # "export USER=john"
#     # "export HOME=/home/john"
#     # "export SHELL=/bin/bash"

#     # # pwd
#     # "pwd /"
#     # "pwd ~"
#     # "pwd .."
#     # "pwd ../../"
#     # "pwd /usr/bin"
#     # "pwd /etc/"
#     # "pwd usr/local/share"


#     # # echo
#     # "echo \"Hello, World!\""
#     # "echo \"This is a test.\""
#     # "echo \"12345\""
#     # "echo \"Special characters: !@#$%^&*()\""
#     # "echo \"Variables: \$VAR_NAME\""
#     # "echo \"Home directory: \$HOME\""
#     # "echo \"Current directory: \$(pwd)\""
# )

# for PARAM in "${INPUT_PARAMS[@]}"; do
#     # Sanitize PARAM for filename by replacing spaces with underscores
#     # SAFE_PARAM=$(echo "$PARAM" | sed 's/ /_/g')
#     SAFE_PARAM=$(echo "$PARAM" | sed 's/ /_/g' | sed 's/["'\'']//g')
#     # Run valgrind, redirecting output to a file with the sanitized PARAM in the filename
#     valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $PROGRAM <<EOF > "/tmp/valgrind_output_${SAFE_PARAM}.txt" 2>&1
# $PARAM
# exit
# EOF
#     # Simplified check for memory leaks
#     if grep -q "definitely lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
#        grep -q "indirectly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
#        grep -q "possibly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt"; then
#         echo "✅ $PARAM"
#     else
#         echo "❌ $PARAM"
#     fi
# done

#!/bin/bash

PROGRAM="./minishell"
# Define your test commands as before
INPUT_PARAMS=(
    "echo hello world"
    "echo \"hello world\""
    "echo 'hello world'"
    "echo hello'world'"
    "echo hello\"\"world"
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
)

# Use a counter to create unique filenames
COUNTER=0

for PARAM in "${INPUT_PARAMS[@]}"; do
    # Use a simpler scheme for filenames
    SAFE_PARAM="test_${COUNTER}"
    COUNTER=$((COUNTER + 1))

    # Run valgrind with the current command, redirecting output to a file
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $PROGRAM <<EOF > "/tmp/valgrind_output_${SAFE_PARAM}.txt" 2>&1
$PARAM
exit
EOF

    # Check for memory leaks
    if grep -q "definitely lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "indirectly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "possibly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt"; then
        echo "✅ Test ${COUNTER}: $PARAM"
    else
        echo "❌ Test ${COUNTER}: $PARAM"
    fi
done
