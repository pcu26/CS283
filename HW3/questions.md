1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice for this shell because it provides efficient line-based input handling. It prevents buffer overflows and handles spaces correctly. It also reads one line at a time, making it easier for the shell to process user commands line by line.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  We use malloc to allocate memory for cmd_buff instead of creating a fixed-size array because it will prevent potential problems and gives us more flexibility. If cmd_buff was a fixed-size array, it would be stored in the stack, which has a limited size. It can cause a stack overflow if the buffer is too large, which will crash the program. Using malloc allows us to allocate memory on the heap, which has more space and allows the program to run safely if the buffer is large. We can also adjust the buffer size if needed.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  It is necessary because it makes sure that the commands are stored correctly and can be executed. If the extra spaces were not removed, commands might have unnecessary whitespace in the beginning or end, which can cause errors when running. The spaces can also affect argument parsing, which will make the shell behave unexpectedly. We make sure that the commands are clean and consistent when trimming spaces.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Redirection is a feature in the shell that allows users to control where input and output go. One common type of redirection is output redirection (> and >>), which sends the standard output of a command to a file. Another type of redirection is input redirection (<), which allows the command to read input from a file instead of the keyboard. Lastly, error redirection (2>) allows error messages to be saved separately from normal output.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection is used to send output to or receive input from a file. Is it mainly used when users want to store command output for later use or process input from a file instead of typing it from the keyboard. Piping is used to to connect the output of one command to the input of another command. This allows data to be processed in a chain. Unlike redirection, piping allows commands to work together in real-time without the need for temporary files. The main difference is redirection interacts with files, while piping connects commands.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: Keeping STDERR separate from STDOUT is important in a shell because it allows users to tell the difference between normal output and error messages. It makes redirection and automation easier. 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our custom shell should handle errors from failed commands by checking their exit status and providing feedback to the user. In Linux, an exit status is returned when a command runs, where 0 means success and any non-zero value indicates an error. The shell should capture the status after each command executes. When an error occurs, a message should print instead of failing without one. The shell should also allow users to control how errors are handled by supporting STDERR redirection, which allows users to separate errors from normal output. Our shell needs to use file descriptor manipulation to ensure that redirections happen correctly before executing a command.
