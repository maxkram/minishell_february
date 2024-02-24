#!/bin/bash

# Define modules or components
declare -a modules=( \
    # "main.c" \
    # "tokenizer" \
    # "parser" \
    # "extender" \
    "execution" \
    # "utils" \
    # "builtins" \
    # "signals"
    )

# Option to delete .dot and .txt files after generation (1 = delete, 0 = keep)
DELETE_FILES=0

# Clear previous call graph
> module_callgraph.txt

# Generate call graph for specified modules
for module in "${modules[@]}"; do
    find "./src/$module" -name "*.c" -exec cflow {} + >> module_callgraph.txt
done

# Convert the call graph to DOT format
python3.10 cflow_to_dot.py module_callgraph.txt module_callgraph.dot

# Generate the visualization with Graphviz
dot -Tpng module_callgraph.dot -o module_callgraph.png

echo "Call graph generated: module_callgraph.png"

# Check if intermediate files should be deleted
if [ "$DELETE_FILES" -eq 1 ]; then
    echo "Deleting intermediate .dot and .txt files..."
    rm -f module_callgraph.txt module_callgraph.dot
    echo "Files deleted."
fi
