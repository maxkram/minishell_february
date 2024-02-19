# import re

# def parse_line(line):
#     """Parse a line from cflow output, extracting indentation, function name."""
#     indent = len(line) - len(line.lstrip())
#     function_name = re.sub(r'\(.*\)', '', line.strip()).split()[0]
#     return indent, function_name

# def main(cflow_file, dot_file):
#     with open(cflow_file, 'r') as f:
#         lines = f.readlines()

#     nodes = set()
#     edges = []
#     parent_stack = []

#     with open(dot_file, 'w') as f:
#         # Write the preamble including graph, node, and edge attributes
#         f.write("digraph callgraph {\n")
#         f.write('    graph [fontsize=30 labelloc="t" label="Call Graph", rankdir="LR"];\n')
#         f.write('    node [shape=box, style=filled, fillcolor=lightgrey];\n')
#         f.write('    edge [color=blue, style=dashed];\n')

#         for line in lines:
#             indent, function_name = parse_line(line)
#             level = indent // 4  # Assuming 4 spaces per indent level

#             # Update stack to current level, last element is parent
#             parent_stack = parent_stack[:level]
#             if parent_stack:
#                 edges.append((parent_stack[-1], function_name))
#             parent_stack.append(function_name)
#             nodes.add(function_name)

#         # Write nodes and edges with possible custom attributes per node/edge
#         for node in nodes:
#             # Example of customizing a specific node, could use conditions here
#             if node == "main":
#                 f.write(f'    "{node}" [label="{node}", shape=ellipse, fillcolor=yellow];\n')
#             else:
#                 f.write(f'    "{node}" [label="{node}"];\n')
#         for src, dst in edges:
#             f.write(f'    "{src}" -> "{dst}";\n')

#         f.write("}\n")

# if __name__ == "__main__":
#     import sys
#     if len(sys.argv) < 3:
#         print("Usage: python cflow_to_dot.py [cflow_output_file] [output_dot_file]")
#     else:
#         main(sys.argv[1], sys.argv[2])

# NEW CODE #


# import re

# def parse_line(line):
#     """Parse a line from cflow output, extracting indentation, function name."""
#     indent = len(line) - len(line.lstrip())
#     function_name = re.sub(r'\(.*\)', '', line.strip()).split()[0]
#     return indent, function_name

# def get_node_attributes(function_name):
#     if function_name.startswith('ft_'):
#         return '[shape=box3d, fillcolor=lightblue]'
#     elif function_name == "main":
#         return '[shape=ellipse, fillcolor=yellow]'
#     else:
#         return '[shape=box, fillcolor=lightgrey]'

# def main(cflow_file, dot_file):
#     with open(cflow_file, 'r') as f:
#         lines = f.readlines()

#     nodes = set()
#     edges = []
#     parent_stack = []

#     with open(dot_file, 'w') as f:
#         f.write("digraph callgraph {\n")
#         f.write('    graph [fontsize=30 labelloc="t" label="call graph", rankdir="LR", nodesep=0.5, ranksep=0.5, splines=ortho];\n')
#         f.write('    node [shape=box, style=filled, fillcolor=lightgrey];\n')
#         f.write('    edge [color=black, style=filled];\n')

#         for line in lines:
#             indent, function_name = parse_line(line)
#             level = indent // 4  # Assuming 4 spaces per indent level

#             parent_stack = parent_stack[:level]
#             if parent_stack:
#                 edges.append((parent_stack[-1], function_name))
#             parent_stack.append(function_name)
#             nodes.add(function_name)

#         for node in nodes:
#             if node == "main":
#                 f.write(f'    "{node}" [label="{node}", shape=ellipse, fillcolor=yellow];\n')
#             else:
#                 # Placeholder for differentiating nodes, you could add conditions here
#                 f.write(f'    "{node}" [label="{node}"];\n')

#         for src, dst in edges:
#             f.write(f'    "{src}" -> "{dst}";\n')

#         f.write("}\n")

# if __name__ == "__main__":
#     import sys
#     if len(sys.argv) < 3:
#         print("Usage: python cflow_to_dot.py [cflow_output_file] [output_dot_file]")
#     else:
#         main(sys.argv[1], sys.argv[2])


# NEW CODE 2 #

import re

def parse_line(line):
    """Parse a line from cflow output, extracting indentation, function name."""
    indent = len(line) - len(line.lstrip())
    function_name = re.sub(r'\(.*\)', '', line.strip()).split()[0]
    return indent, function_name

def get_node_attributes(function_name, is_leaf=False):
    """Return custom node attributes based on the function name and whether it's a leaf node."""
    if is_leaf and not function_name.startswith('ft_'):
        return 'shape=box, style=filled, fillcolor="#ff9980"'  # Color leaf nodes differently
    elif function_name.startswith('ft_'):
        return 'shape=box, fillcolor=lightblue'
    elif function_name == "main":
        return 'shape=ellipse, fillcolor=yellow, style=filled'  # Highlight main function
    else:
        return 'shape=box, style=filled, fillcolor=lightgrey'  # Default attributes for other functions


def get_edge_attributes(src, dst):
    """Return custom edge attributes based on source and destination."""
    # Example: Differentiate certain types of calls with color or style
    # This is a placeholder for customization; you can add conditions based on your analysis
    return 'color=lightgrey, style=filled'

def main(cflow_file, dot_file):
    with open(cflow_file, 'r') as f:
        lines = f.readlines()

    nodes = set()
    edges = []
    parent_stack = []
    leaf_nodes = set()  # Set to store leaf nodes (functions with no outgoing edges)

    with open(dot_file, 'w') as f:
        f.write("digraph callgraph {\n")
        f.write('    graph [fontsize=30 labelloc="t" label="Call Graph", rankdir="LR", nodesep=0.75, ranksep=0.75, splines=ortho];\n')
        f.write('    node [fontsize=12];\n')
        f.write('    edge [fontsize=12];\n')

        for line in lines:
            indent, function_name = parse_line(line)
            level = indent // 4  # Assuming 4 spaces per indent level

            # Update stack to current level, last element is parent
            parent_stack = parent_stack[:level]
            if parent_stack:
                edges.append((parent_stack[-1], function_name))
            parent_stack.append(function_name)
            nodes.add(function_name)

        # Identify leaf nodes (functions with no outgoing edges)
        for src, dst in edges:
            leaf_nodes.discard(src)  # Remove nodes with outgoing edges
            leaf_nodes.add(dst)      # Add nodes that are destinations of edges

        # Write nodes with custom attributes
        for node in nodes:
            if node in leaf_nodes:
                node_attributes = get_node_attributes(node, is_leaf=True)
            else:
                node_attributes = get_node_attributes(node, is_leaf=False)
            f.write(f'    "{node}" [{node_attributes}];\n')

        # Write edges with custom attributes
        for src, dst in edges:
            edge_attributes = get_edge_attributes(src, dst)
            f.write(f'    "{src}" -> "{dst}" [{edge_attributes}];\n')

        f.write("}\n")

if __name__ == "__main__":
    import sys
    if len(sys.argv) < 3:
        print("Usage: python cflow_to_dot.py [cflow_output_file] [output_dot_file]")
    else:
        main(sys.argv[1], sys.argv[2])
