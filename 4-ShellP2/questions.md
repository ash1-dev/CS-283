1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**: By using fork/execvp instead of just calling it allows fork to create a new child process, whereas execvp only replaces the current process with a new process.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**: If the fork system call fails it returns -6 and the child process is not created.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**: Execvp() finds the command to execute by searching the directories listed in the PATH environment variable

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**: The purpose of calling wait() in the parent process is to wait for the child process to finish execution before the parent continues.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**: WEXITSTATUS() is used to extract the exit status of a child process that has finished execution, which is important because it indicates whether the command executed successfully.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: I made a helper function to help parse the arguments called trim_spaces(). In trim_spaces(), quoted arguments are handled by tracking whether the current character is inside quotes. When inside quotes, spaces are allowed, and the argument is not split on spaces.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**: In this implementation, I added better handling of quoted arguments since that was neccessary to the assignment and I created a helper function so I can bring it along with me to future assignments. There were not unexpected challenges in refactoring my old code.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**: Signals in a Linux system serve the purpose of notifying that a specific event has occured. Unnlike other IPC signals are typically used to handle specific events rather than exchanging large amounts of data.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**: SIGKILL immediately terminates a process without giving it a chance to clean up, it is typically used when a process is unresponsive or cannot be terminated by other signals. SIGTERM requests a process to terminate, allowing it to perform cleanup tasks before exiting, it is typically used by system administrators or other processes to gracefully shut down a process. SIGINT is sent when the user interrupts a process by pressing Ctrl+C in the terminal, typically used to interrupt a running process.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**: SIGSTOP is used to stop a process without terminating it. It cannot be caught or ignored like SIGINT because it is a kernel level signal designed to control process execution at the system level.
