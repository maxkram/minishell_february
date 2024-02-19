import re

def parse_line(line):
    """Parse a line from cflow output, extracting indentation and function name."""
    indent = len(line) - len(line.lstrip())
    function_name = re.sub(r'\(.*\)', '', line.strip()).split()[0]
    return indent, function_name

def find_callers(cflow_file, target_function):
    """Find all functions that call the target function."""
    callers = set()

    with open(cflow_file, 'r') as f:
        lines = f.readlines()

    for line in lines:
        indent, function_name = parse_line(line)
        if function_name == target_function:
            # If the target function is called, add the previous function to callers set
            for prev_line in reversed(lines[:lines.index(line)]):
                prev_indent, prev_function_name = parse_line(prev_line)
                if prev_indent < indent:
                    callers.add(prev_function_name)
                    break

    return callers

def main(cflow_file, target_function):
    callers = find_callers(cflow_file, target_function)

    # Generate call graph with only functions that call the target function
    with open(cflow_file, 'r') as f:
        lines = f.readlines()

    nodes = set()
    edges = set()

    for line in lines:
        indent, function_name = parse_line(line)
        if function_name in callers:
            nodes.add(function_name)
            edges.add((function_name, target_function))

    # Write the filtered call graph to a DOT file
    dot_file = target_function + "_callers.dot"
    with open(dot_file, 'w') as f:
        f.write("digraph callgraph {\n")
        for node in nodes:
            f.write(f'    "{node}" [label="{node}"];\n')
        for src, dst in edges:
            f.write(f'    "{src}" -> "{dst}";\n')
        f.write("}\n")

    print(f"Filtered call graph generated: {dot_file}")

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 3:
        print("Usage: python call_graph_visualizer.py [cflow_output_file] [target_function]")
    else:
        main(sys.argv[1], sys.argv[2])
