1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  When we use fork() before execvp(), it allows the shell to create a new child process to run the command while keeping the original shell running. This is important because if we called execvp() directly, the shell itself would be replaced by the command, making it impossible to run more commands afterward. By using fork(), the shell can stay active and run multiple commands, allowing users to type new commands.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork() system call fails, it means that the operating system was unable to create a new process. In my implementation, this scenario is handled by checking if fork() returns a negative value.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  The execvp() function searches for the command to execute by looking through the directories listed in the PATH environment variable. When a command is given (e.g., "ls"), execvp() checks if it is an absolute or relative path. If not, it iterates through each directory in PATH, attempting to execute the command from those locations. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The purpose of using wait() in the parent process after forking is to ensure that the parent waits for the child process to complete. This prevents situations where the child process terminates but remains in the table because the parent didn't collect the exit status. If we didn't use wait(), the parent process will continue to run without knowing the termination of the child process.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATUS() extracts the exit status of a terminated child process from the integer status from wait(). This allows the parent process to determine if the child executed successfully or had an error.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  My implemention handles quoted arguments by recognizing when a command argument is enclosed in single or double quotes. This is necessary because arguments that contain spaces should be treated as a single entity, rather than being split.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: In this assignment, I changed the parsing logic by properly handling quoted arguments. I made sure that the spaces within the squotes are preserved as part of a single argument. One of the unexpected challenges that I faced was handling quotes where the user forgets to close the quote. Before, the parser did not detect this error correctly.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals in a Linux system are used to notify processes of specific events. Other IPC methods can be used for data exchange and coordination.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL is used to forcefully terminate a process. SIGTERM is used to request a process to terminate gracefully. SIGINT is an interactive interrupt.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  When a process receives SIGSTOP, it is immediately paused by the operating system. It can't be continued unless there is a SIGCONT. SIGSTOP can't be caught or ignored because it is always enforced by the kernel.
