#!/bin/bash

# Command to test
COMMAND="echo Hello World"

# Expected output lines (adjust according to your shell's behavior and output format)
EXPECTED_OUTPUT_LINES=(
    "minishell\$ $COMMAND"
    "Hello World"
    "minishell\$ exit"
)

# Function to join array elements into a string
join_by() { local IFS="$1"; shift; echo "$*"; }

# Expected output as a single string
EXPECTED_OUTPUT=$(join_by $'\n' "${EXPECTED_OUTPUT_LINES[@]}")

# Execute the command in minishell and capture the output
ACTUAL_OUTPUT=$(printf "%s\nexit\n" "$COMMAND" | ./minishell)

# Compare actual output to expected output
if [ "$ACTUAL_OUTPUT" == "$EXPECTED_OUTPUT" ]; then
    echo "Test passed!"
else
    echo "Test failed."
    echo "----------------"
    echo "Expected output:"
    echo "$EXPECTED_OUTPUT"
    echo "----------------"
    echo "Actual output:"
    echo "$ACTUAL_OUTPUT"
fi
