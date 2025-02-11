#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    if (cmd_line == NULL || strlen(cmd_line) == 0)
    {
        return WARN_NO_CMDS;
    }

    memset(clist, 0, sizeof(command_list_t));  

    //define variables
    char *token;
    char *saveptr;
    int cmd_count = 0;

    //split commands by pipes
    token = strtok_r(cmd_line, PIPE_STRING, &saveptr);
    while (token != NULL)
    {
        //enforce command limit
        if (cmd_count >= CMD_MAX)
        {
            return ERR_TOO_MANY_COMMANDS;
        }

        //trim spaces
        while (*token == SPACE_CHAR)
            token++;

        char *end = token + strlen(token) - 1;
        while (end > token && *end == SPACE_CHAR)
            *end-- = '\0';

        //split arguments
        char *arg_ptr = strchr(token, SPACE_CHAR);
        if (arg_ptr != NULL)
        {
            *arg_ptr = '\0';
            arg_ptr++;
        }
        else
        {
            arg_ptr = NULL; //no arguments
        }

        //check length boundaries
        if (strlen(token) >= EXE_MAX || (arg_ptr && strlen(arg_ptr) >= ARG_MAX))
        {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strcpy(clist->commands[cmd_count].exe, token);
        if (arg_ptr)
        {
            strcpy(clist->commands[cmd_count].args, arg_ptr);
        }
        else
        {
            clist->commands[cmd_count].args[0] = '\0'; //empty string with no arguments
        }
        
        cmd_count++;

        token = strtok_r(NULL, PIPE_STRING, &saveptr);
    }

    clist->num = cmd_count;
    return OK;
}
