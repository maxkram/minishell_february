#!/bin/bash

# Check if a command file was provided as an argument
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <command_file>"
    exit 1
fi

command_file="$1"

# Check if the command file exists
if [ ! -f "$command_file" ]; then
    echo "Command file $command_file not found."
    exit 1
fi

# Initialize command counter
command_number=0

# Read commands from the file and execute them
while IFS= read -r cmd; do
    # Ignore empty lines and lines starting with #
    if [[ -z "$cmd" || "$cmd" =~ ^# ]]; then
        continue
    fi

    # Increment command counter
    command_number=$((command_number + 1))

    # echo "Testing command: $cmd"

    # Run the command using Bash and save the output
    echo $cmd | bash > bash_output.txt 2>&1

    # Run the command using your minishell and save the output
    echo $cmd | ./minishell > minishell_output_with_prompt.txt 2>&1

    # Use sed to remove the custom minishell prompt from the output
    sed '/^\$ /d' minishell_output_with_prompt.txt > minishell_output.txt

    # Compare the outputs
    if diff bash_output.txt minishell_output.txt > /dev/null; then
        echo "✅ Test $command_number: $cmd"
    else
        echo "❌ Test $command_number: $cmd"
        echo "Debugging output for Bash:"
        cat bash_output.txt
        echo "Debugging output for Minishell:"
        cat minishell_output.txt
    fi

done < "$command_file"

# Optionally, comment out the cleanup if you want to manually inspect files later
rm bash_output.txt minishell_output_with_prompt.txt minishell_output.txt
