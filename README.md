
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
