#!/bin/bash

PROGRAM="./minishell"
INPUT_PARAMS=("ls" "echo Hello" "pwd")

for PARAM in "${INPUT_PARAMS[@]}"; do
    echo "Testing with input: $PARAM"
    # Sanitize PARAM for filename by replacing spaces with underscores
    SAFE_PARAM=$(echo "$PARAM" | sed 's/ /_/g')
    # Run valgrind, redirecting output to a file with the sanitized PARAM in the filename
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $PROGRAM <<EOF > "/tmp/valgrind_output_${SAFE_PARAM}.txt" 2>&1
$PARAM
exit
EOF
    # Simplified check for memory leaks
    if grep -q "definitely lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "indirectly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt" && \
       grep -q "possibly lost: 0 bytes in 0 blocks" "/tmp/valgrind_output_${SAFE_PARAM}.txt"; then
        echo "No memory leaks for input: $PARAM"
    else
        echo "Memory leaks detected for input: $PARAM"
    fi
    echo "--------------------------------"
done
