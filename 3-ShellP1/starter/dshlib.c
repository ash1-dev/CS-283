#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    char *token;
    char *saveptr;
    int cmd_count = 0;

    // Initialize the command list
    memset(clist, 0, sizeof(command_list_t));

    // Split the command line by pipe character
    token = strtok_r(cmd_line, PIPE_STRING, &saveptr);
    while (token != NULL) {
        if (cmd_count >= CMD_MAX) {
            return ERR_TOO_MANY_COMMANDS;
        }

        // Remove leading and trailing spaces
        while (isspace((unsigned char)*token)) token++;
        char *end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) end--;
        *(end + 1) = '\0';

        // Split the command into executable and arguments
        char *exe = strtok(token, " ");
        char *args = strtok(NULL, "");

        if (exe == NULL) {
            return WARN_NO_CMDS;
        }

        if (strlen(exe) >= EXE_MAX || (args != NULL && strlen(args) >= ARG_MAX)) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        // Copy the executable and arguments into the command list
        strncpy(clist->commands[cmd_count].exe, exe, EXE_MAX);
        if (args != NULL) {
            strncpy(clist->commands[cmd_count].args, args, ARG_MAX);
        } else {
            clist->commands[cmd_count].args[0] = '\0';
        }

        cmd_count++;
        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }

    clist->num = cmd_count;
    if (clist->num == 0)
    {
        return WARN_NO_CMDS;
    }
    
    return OK;
}