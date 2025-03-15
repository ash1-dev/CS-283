#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/**** 
 **** FOR REMOTE SHELL USE YOUR SOLUTION FROM SHELL PART 3 HERE
 **** THE MAIN FUNCTION CALLS THIS ONE AS ITS ENTRY POINT TO
 **** EXECUTE THE SHELL LOCALLY
 ****
 */

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    memset(cmd_buff, 0, sizeof(cmd_buff_t));
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX * sizeof(char));
    if (!cmd_buff->_cmd_buffer) {
        return ERR_MEMORY;
    }
    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff->_cmd_buffer) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL;
    }
    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    memset(cmd_buff->_cmd_buffer, 0, SH_CMD_MAX);
    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));
    return OK;
}

int free_cmd_list(command_list_t *cmd_lst) {
    for (int i = 0; i < cmd_lst->num; i++) {
        free_cmd_buff(&cmd_lst->commands[i]);
    }
    return OK;
}

void trim_spaces(char *str) {
    int i, j = 0;
    int in_quotes = 0;
    int space_seen = 0;
    char temp[SH_CMD_MAX] = {0};

    i = 0;
    while (isspace(str[i])) i++;

    for (; str[i] != '\0'; i++) {
        if (str[i] == '"') {
            in_quotes = !in_quotes;
        }

        if (!in_quotes && isspace(str[i])) {
            if (!space_seen) {
                temp[j++] = ' ';
                space_seen = 1;
            }
        } else {
            temp[j++] = str[i];
            space_seen = 0;
        }
    }

    if (j > 0 && temp[j - 1] == ' ') {
        temp[j - 1] = '\0';
    } else {
        temp[j] = '\0';
    }

    strcpy(str, temp);
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (cmd_line == NULL || strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    trim_spaces(cmd_line);

    cmd_buff->argc = 0;
    memset(cmd_buff->argv, 0, sizeof(cmd_buff->argv));

    char *token = cmd_line;
    int in_quotes = 0;

    while (*token != '\0') {
        while (*token == ' ' && !in_quotes) token++;

        if (*token == '"') {
            in_quotes = 1;
            token++;
        }

        cmd_buff->argv[cmd_buff->argc++] = token;

        while (*token != '\0' && (in_quotes || *token != ' ')) {
            if (*token == '"' && in_quotes) {
                in_quotes = 0;
                *token = '\0';
            }
            token++;
        }

        if (*token != '\0') {
            *token = '\0';
            token++;
        }

        if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {
            break;
        }
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;
    return OK;
}

int handle_cd(cmd_buff_t *cmd) {
    if (cmd->argc < 2) {
        return OK;
    }

    char path[SH_CMD_MAX] = {0};
    for (int i = 1; i < cmd->argc; i++) {
        strcat(path, cmd->argv[i]);
        if (i < cmd->argc - 1) {
            strcat(path, " ");
        }
    }

    if (chdir(path) != 0) {
        perror("dsh");
        return ERR_EXEC_CMD;
    }
    return OK;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (cmd->argc == 0 || cmd->argv[0] == NULL) {
        return BI_NOT_BI;
    }
    if (strcmp(cmd->argv[0], "exit") == 0) {
        free_cmd_buff(cmd);
        exit(OK);
    } else if (strcmp(cmd->argv[0], "cd") == 0) {
        return handle_cd(cmd) == OK ? BI_EXECUTED : ERR_EXEC_CMD;
    }
    return BI_NOT_BI;
}

int exec_cmd(cmd_buff_t *cmd) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {  // Child process
        if (execvp(cmd->argv[0], cmd->argv) == -1) {
            fprintf(stderr, "dsh: command not found: %s\n", cmd->argv[0]);
            exit(ERR_EXEC_CMD);
        }
    } else if (pid < 0) {
        perror("dsh");
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *cmd_copy = strdup(cmd_line);  
    if (!cmd_copy) return ERR_MEMORY;
    
    char *token = strtok(cmd_copy, "|");
    int cmd_index = 0;

    while (token != NULL && cmd_index < CMD_MAX) {
        trim_spaces(token);
        
        if (alloc_cmd_buff(&clist->commands[cmd_index]) != OK) {
            free(cmd_copy);
            return ERR_MEMORY;
        }
        
        strncpy(clist->commands[cmd_index]._cmd_buffer, token, SH_CMD_MAX);
        clist->commands[cmd_index]._cmd_buffer[SH_CMD_MAX - 1] = '\0';  
        
        if (build_cmd_buff(clist->commands[cmd_index]._cmd_buffer,
                           &clist->commands[cmd_index]) == OK) {
            cmd_index++;
        }
        token = strtok(NULL, "|");
    }

    clist->num = cmd_index;
    free(cmd_copy); 
    return (cmd_index > 0) ? OK : WARN_NO_CMDS;
}

int execute_pipeline(command_list_t *clist) {
    int num_cmds = clist->num;
    int pipefds[2 * (num_cmds - 1)];

    // Create pipes
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) == -1) {
            perror("pipe");
            return ERR_MEMORY;
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();
        if (pid == 0) {  // Child process
            if (i > 0) {
                dup2(pipefds[(i - 1) * 2], STDIN_FILENO);  // Read from previous pipe
            }
            if (i < num_cmds - 1) {
                dup2(pipefds[i * 2 + 1], STDOUT_FILENO);  // Write to next pipe
            }

            // Close all pipe fds
            for (int j = 0; j < 2 * (num_cmds - 1); j++) {
                close(pipefds[j]);
            }

            // Execute command
            if (execvp(clist->commands[i].argv[0], clist->commands[i].argv) == -1) {
                perror("execvp");
                exit(ERR_EXEC_CMD);
            }
        }
    }

    for (int i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipefds[i]);
    }
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }

    return OK;
}

int exec_local_cmd_loop() {
    char *cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
    if (!cmd_buff) {
        return ERR_MEMORY;
    }

    cmd_buff_t cmd;
    if (alloc_cmd_buff(&cmd) != OK) {
        free(cmd_buff);
        return ERR_MEMORY;
    }

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        char *orig_cmd = strdup(cmd_buff);
        if (!orig_cmd) {
            fprintf(stderr, "Error duplicating command string\n");
            continue;
        }

        bool has_pipe = (strchr(orig_cmd, PIPE_CHAR) != NULL);

        if (has_pipe) {
            command_list_t clist;
            if (build_cmd_list(orig_cmd, &clist) == OK && clist.num > 0) {
                execute_pipeline(&clist);
            } else {
                printf("Error: Invalid pipeline format\n");
            }
        } else {
            if (build_cmd_buff(cmd_buff, &cmd) != WARN_NO_CMDS && cmd.argc > 0) {
                if (exec_built_in_cmd(&cmd) == BI_NOT_BI) {
                    exec_cmd(&cmd);
                }
            }
        }

        free(orig_cmd);
    }

    free_cmd_buff(&cmd);
    free(cmd_buff);
    return OK;
}
