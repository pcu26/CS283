#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

int main()
{
    char cmd_buff[ARG_MAX];     
    int rc = 0;
    command_list_t clist;

    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        //remove the trailing \n from cmd_buff
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        //IMPLEMENT THE REST OF THE REQUIREMENTS

        //check for exit command
        if (strcmp(cmd_buff, EXIT_CMD) == 0)
        {
            exit(OK);
        }

        //parse command using build_cmd_list
        rc = build_cmd_list(cmd_buff, &clist);

    
        if (rc == WARN_NO_CMDS)
        {
            printf("%s\n", CMD_WARN_NO_CMD);
        }
        else if (rc == ERR_TOO_MANY_COMMANDS)
        {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        }
        else
        {
            printf(CMD_OK_HEADER, clist.num);
            for (int i = 0; i < clist.num; i++)
            {
                //fix output format
                if (strlen(clist.commands[i].args) > 0)
                {
                    printf("<%d>%s[%s]\n", i + 1, clist.commands[i].exe, clist.commands[i].args);
                }
                else
                {
                    printf("<%d>%s\n", i + 1, clist.commands[i].exe);
                }
            }
        }
    }

    return OK;
}

