1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

When I run the piped commands, the shell creates a new process for each commands and connects the commands so they can pass data to each other. To ensure that the commands finish before the prompt again, the shell will wait for each process to complete using waitpid(). This will stop the shell from letting the user type more commands while previous commands are still running. If you forget to call waitpid(), it can cause the shell to return to the prompt too soon, making it seem like the command finished when it didn't.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

The dup2() function is used to redirect input and output files descriptors so that the commands in a pipeline can communicate properly. Closing unused pipe ends after calling dup2() is necessary to prevent leaks and makes sure the pipe's behavior is proper. If the pipes are open, it may never receive an end of file signal, which will cause a wait for an input that will never come.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is implemented as a built-in command because it directly modifies the current directory of the shell.  Cd needs to change the directory of the shell in itself, and not a child process like external commands. If cd was implemented as an external process, it would change the directory inside of the child process. This will make it terminate.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To allow an arbitrary number of piped commands instead of a fixed limit, I will have to modify the implementation to use dynamic memory allocation to store commands and pipes. To allocate memory, I will use malloc() and expand it with realloc() when there are more commands for parsing. The memory would have to be freed to prevent any memory leaks. Some trade-offs can be some more complexity in managing memory. 
