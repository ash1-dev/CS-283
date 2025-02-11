1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  'fgets()' is a good choice for this application since it has a maximum size built into the function. Since we have a SH_CMD_MAX variable we can ensure that input is not larger than that size.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  'malloc()' is a good choice instead of a fixed-size array since the memory allocated from a malloc has dynamic storage duration and will exist until freed, unlike an array which only exists as long as the function that calls it.


3. In `dshlib.c`, the function `build_cmd_list()` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  'build_cmd_list()' trims leading and trailing spaces from each command before storing it because leading spaces could lead to the first token being empty space and trailing spaces might cause issues with parsing arguments.

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Output Redirection (>): ls > output.txt
                    Redirects output of ls to file output.txt.
                    This requires opening the file in write mode and replacing STDOUT with the file descriptor for output.txt
                   Input Redirection (<): sort < input.txt
                    Redirects input of sort to input.txt
                    Implementing this requires opening the file in read mode and replacing STDIN with the file descriptor for input.txt
                   Appending Output (>>): echo "new line" >> output.txt
                    Appends the output of echo to the end of output.txt instead of overwriting it.
                    Implementing this requires opening the file in append mode and ensuring the file pointer is positioned at the end.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Pipes directly connects the output of one command to the input of another command, unlike redirection which redirects input or ouput to be from/to a file.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  Keeping these separate allows for a log of all the errors that occured.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  Our custom shell should not merge them since having a log of errors is useful when debugging an issue.