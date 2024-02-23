
1. execute_pipeline
	a. create_pipes
	b. while commands left
		i.		fork
		ii.		create_child_process
		iii. 	close_unused_pipe_ends
	c. close_all_pipe_fds
	d. cleanup_pipes_and_wait

# 1. execute_pipeline
    Initializes the execution of a series of commands as separate processes, connected through pipes.
    This function sets up the infrastructure for inter-process communication and manages the lifecycle of each process in the pipeline.
## a. create pipes
    Allocates pipes for communication between the processes in the pipeline. Each pipe allows data to flow from one process's output to the next process's input.
## b. while commands left
    Iterates through each command that needs to be executed as part of the pipeline. For each command, the following steps are performed:
### i.   fork
    Creates a new process. The parent process will continue to iterate and set up the pipeline, while the child process will execute a specific command in the pipeline.
### ii.  create_child_process
    Responsible for setting up the child process's environment to execute a command, including redirecting standard input/output as necessary for the pipeline's operation.
### iii. close_unused_pipe_ends
    Closes pipe file descriptors that are not used by the current process to prevent resource leaks and ensure proper data flow through the pipeline.
##  c. close_all_pipe_fds
    Closes all remaining open pipe file descriptors in the parent process after all child processes have been forked, ensuring no unused file descriptors remain open.
##	d. cleanup_pipes_and_wait
    Performs any necessary cleanup of resources, such as deallocating memory or closing file descriptors. Waits for all child processes to complete, ensuring the parent process only proceeds after the entire pipeline has finished executing.s


``` shell
cat access.log | awk '{print $1}' | sort | uniq -c | sort -nr | awk '{print $2, "requests:", $1}'
find . -type f | rev | cut -d. -f1 | rev | sort | uniq -c | sort -nr | awk '{print $2, "files:", $1}'
ps aux | sort -nk +4 | tail | awk '{print $11, "memory usage:", $4"%"}'
git log --pretty=format:%aN | sort | uniq -c | sort -nr | awk '{print $2, "lines changed:", $1}'
find . -type f | xargs -L1 dirname | sort | uniq -c | sort -nr | awk '{print $2, "files:", $1}'
netstat -ntu | awk '{print $5}' | cut -d: -f1 | sort | uniq -c | sort -nr | awk '{print $2, "connections:", $1}'
grep -rI "pattern" . | wc -l | sort -nr | awk '{print $2, "occurrences:", $1}'
grep -rI "pattern" . | cut -d: -f1 | uniq -c | sort -nr | awk '{print $2, "occurrences:", $1}'
```


# redirection tests

Input redirection can be combined with other shell features to enhance its utility. Here are some common combinations and scenarios where input redirection is used alongside other redirections and shell operations:

1. **Combining Input and Output Redirection:**
   - You can redirect both input and output of a command. The input is read from a file, and the output is written to another file.
   ```bash
   command < inputFile > outputFile
   ```

2. **Piping with Input Redirection:**
   - Commands can be piped (`|`) together with input redirection to feed a file's contents into the first command in a pipeline.
   ```bash
   command1 < inputFile | command2 > outputFile
   ```
   - This feeds the contents of `inputFile` to `command1`, then pipes the output of `command1` into `command2`, with the final output written to `outputFile`.

3. **Here Documents (Heredoc):**
   - A here document is a special form of redirection that allows you to provide input to a command directly in the command line or script.
   ```bash
   command <<END
   input line 1
   input line 2
   END
   ```
   - This directs the lines between `<<END` and `END` as input to `command`. The `END` marker can be any word you choose.

4. **Appending Output While Redirecting Input:**
   - You can redirect input from a file and append the output to another file using `>>`.
   ```bash
   command < inputFile >> outputFile
   ```
   - This reads input from `inputFile` and appends the output to `outputFile`.

5. **Using File Descriptors:**
   - Advanced usage includes manipulating file descriptors directly. For example, redirecting standard error (`stderr`) to standard output (`stdout`) while also redirecting input.
   ```bash
   command < inputFile > outputFile 2>&1
   ```
   - This redirects both the standard output and standard error of `command` to `outputFile`, with input taken from `inputFile`.

6. **Input Redirection for Command Substitution:**
   - While not a direct form of input redirection, command substitution (`$(command)`) can be used in conjunction with input redirection to use the output of one command as input to another.
   ```bash
   command1 < $(command2)
   ```
   - Note: This is a conceptual example. The actual syntax for using command substitution as input would depend on the specific use case, such as passing the output of `command2` as an argument to `command1`.

7. **Redirection in Loops and Conditionals:**
   - Within shell scripts, input redirection can be used inside loops or conditional statements to process files or command output in a controlled manner.
   ```bash
   while read line; do
       command "$line"
   done < inputFile
   ```
   - This reads lines from `inputFile` and processes them one at a time with `command`.

These combinations showcase the flexibility of shell redirection mechanisms, allowing for complex input and output flows in shell commands and scripts.

[ ] norminette
[ ] refactor execution
[ ] fix heredoc
[ ] export does not print env
[ ] fix export GHOST=123 | env | grep GHOST
[ ] replace ft_printf_fd


# concerns
[ ] in extender different path to include #include "../include/minishell.h"
[ ] inside signals one more global variable int global_signal = 0;

## heredoc
- [ ] bash in heredoc mode expand variables even with single and doudble quotes.

minishell$ cat << EOF
> $USER
> "$USER"
> '$USER'
> EOF
device
"$USER"
'$USER'
