#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "dshlib.h"

int exec_local_cmd_loop() {
    char cmd_buff[SH_CMD_MAX];  //buffer for user input
    cmd_buff_t cmd;

    while (1) {
        printf("%s", SH_PROMPT);
        fflush(stdout);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        // remove trailing lines
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // allocate memory for command buffer
        if (alloc_cmd_buff(&cmd) != OK) {
            fprintf(stderr, "Memory allocation error\n");
            continue;
        }

        // parse input
        if (build_cmd_buff(cmd_buff, &cmd) != OK) {
            fprintf(stderr, "error: command parsing failed\n");
            free_cmd_buff(&cmd);
            continue;
        }

        // check for any build in commands
        Built_In_Cmds bi_cmd = exec_built_in_cmd(&cmd);
        if (bi_cmd == BI_EXECUTED) {
            free_cmd_buff(&cmd);
            continue;  
        }

        // fork if not a built in command
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork error");
            free_cmd_buff(&cmd);
            exit(ERR_MEMORY);
        }
        else if (pid == 0) {
            execvp(cmd.argv[0], cmd.argv);
            perror("command execution error");
            free_cmd_buff(&cmd);
            exit(ERR_EXEC_CMD);
        }
        else {
            int status;
            waitpid(pid, &status, 0);
        }

        free_cmd_buff(&cmd);  // free memory
    }

    return OK;
}

Built_In_Cmds exec_built_in_cmd(cmd_buff_t *cmd) {
    if (cmd->argc == 0) {
        return BI_NOT_BI;
    }

    if (strcmp(cmd->argv[0], "cd") == 0) {
        if (cmd->argc == 1) {
            return BI_EXECUTED;  // do nothing
        }
        if (cmd->argc == 2) {
            if (chdir(cmd->argv[1]) != 0) {
                perror("cd error");
            }
            return BI_EXECUTED;
        }
        fprintf(stderr, "cd: too many arguments\n");
        return BI_EXECUTED;
    }

    if (strcmp(cmd->argv[0], "pwd") == 0) {
        char cwd[ARG_MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);  //ensures newline
        } else {
            perror("pwd error");
        }
        return BI_EXECUTED;
    }

    if (strcmp(cmd->argv[0], EXIT_CMD) == 0) {
        exit(OK_EXIT);
    }

    return BI_NOT_BI;
}

 //allocate memory for cmd_buff_t
 
int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    if (!cmd_buff) {
        return ERR_MEMORY;
    }

    memset(cmd_buff, 0, sizeof(cmd_buff_t)); 
    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);

    if (!cmd_buff->_cmd_buffer) {
        return ERR_MEMORY;
    }

    // make sure that all argv pointers are null
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd_buff->argv[i] = NULL;
    }

    return OK;
}

//free memory
int free_cmd_buff(cmd_buff_t *cmd_buff) {
    if (cmd_buff && cmd_buff->_cmd_buffer) {
        free(cmd_buff->_cmd_buffer);
        cmd_buff->_cmd_buffer = NULL; 
    }
    memset(cmd_buff, 0, sizeof(cmd_buff_t));  
    return OK;
}

//parse input command line into cmd_buff_t
int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (!cmd_line || !cmd_buff || !cmd_buff->_cmd_buffer) {
        return ERR_CMD_ARGS_BAD;
    }

    cmd_buff->argc = 0;
    char *ptr = cmd_buff->_cmd_buffer;  // use buffer
    char *src = cmd_line;
    bool in_quotes = false;
    
    while (*src) {
        // skip leading spaces
        while (isspace((unsigned char)*src) && !in_quotes) {
            src++;
        }

        if (*src == '\0') {
            break;
        }

        
        if (*src == '"' || *src == '\'') {
            in_quotes = true;
            src++;  // skip quote
        }

        cmd_buff->argv[cmd_buff->argc] = ptr;  // store argument position

        while (*src && (in_quotes || !isspace((unsigned char)*src))) {
            if (*src == '"' || *src == '\'') {
                in_quotes = !in_quotes;  
            } else {
                *ptr++ = *src;
            }
            src++;
        }

        *ptr++ = '\0';  
        cmd_buff->argc++;

        if (*src) {
            src++;  
        }
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;  
    return OK; 
}
