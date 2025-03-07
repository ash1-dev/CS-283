1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My implementation ensures that all child processes complete before the shell continues by using waitpid() for each child process, making sure the shell does not prompt for new input until all parts of the pipeline have completed execution. If I forgot to call waitpid() on all child processes, those processes might remaind as unfinished and over time this could cause resource leaks and even unpredictable behavior.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

It is necessary to close unused pipe ends after calling dup2() to signal the EOF to reading processes. If the write end of a pipe remains open, the reading process will not see EOF and could potentially cause it to wait for more input.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

By implementing cd as a built-in command, the shell can directly modify its own environment. If it were implemented as an external command, it would run in a child process which would not affect the parent shell. This would not change the parent shells directory, which is the point of the command.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To support an arbitrary number of piped commands, I would need to replace fixed-size arrays with dynamically allocated data structures using malloc(), allowing it to scale based on the number of pipes provided. Some trade-offs to consider are: this would increase memory management complexity since I now need to avoid leaks and free memory and users could now provide extremely long pipelines which would also require me to provide limits to prevent excessive resource usage.
