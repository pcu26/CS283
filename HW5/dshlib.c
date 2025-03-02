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
    char cmd_buff[SH_CMD_MAX];  // this is the buffer
    command_list_t cmd_list;
    int exit_status = OK;

    while (1) {
        printf("%s", SH_PROMPT);
        fflush(stdout);

        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        // remove trailing new line
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // exit command
        if (strcmp(cmd_buff, "exit") == 0) {
            printf("exiting...\n");
            return exit_status;
        }

        // parsing
        if (build_cmd_list(cmd_buff, &cmd_list) != OK) {
            fprintf(stderr, "error: command parsing failed\n");
            continue;
        }

        exit_status = execute_pipeline(&cmd_list);
    }
    return exit_status;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    if (!cmd_line || !clist) {
        return ERR_CMD_ARGS_BAD;
    }

    clist->num = 0;
    char *token = strtok(cmd_line, PIPE_STRING);
    while (token != NULL && clist->num < CMD_MAX) {
        cmd_buff_t *cmd = &clist->commands[clist->num];
        if (build_cmd_buff(token, cmd) != OK) {
            return ERR_CMD_ARGS_BAD;
        }
        clist->num++;
        token = strtok(NULL, PIPE_STRING);
    }

    if (clist->num == 0) {
        return WARN_NO_CMDS;
    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    if (!cmd_line || !cmd_buff) {
        return ERR_CMD_ARGS_BAD;
    }

    // allocate memory
    memset(cmd_buff, 0, sizeof(cmd_buff_t));

    cmd_buff->_cmd_buffer = malloc(SH_CMD_MAX);
    if (!cmd_buff->_cmd_buffer) {
        return ERR_MEMORY;
    }

    cmd_buff->argc = 0;
    char *ptr = cmd_buff->_cmd_buffer;
    char *src = cmd_line;
    bool in_quotes = false;

    while (*src) {
        while (isspace((unsigned char)*src) && !in_quotes) {
            src++;
        }

        if (*src == '\0') {
            break;
        }

        if (*src == '"' || *src == '\'') {
            in_quotes = !in_quotes;
            src++;
        }

        cmd_buff->argv[cmd_buff->argc] = ptr;

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

        if (cmd_buff->argc >= CMD_ARGV_MAX - 1) {  // will prevent overflow
            break;
        }

        if (*src) {
            src++;
        }
    }

    cmd_buff->argv[cmd_buff->argc] = NULL;
    return OK;
}

int execute_pipeline(command_list_t *clist) {
    int num_cmds = clist->num;
    int pipes[num_cmds - 1][2];
    pid_t pids[num_cmds];

    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("pipe error");
            return ERR_MEMORY;
        }
    }

    for (int i = 0; i < num_cmds; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("fork error");
            return ERR_MEMORY;
        }
        else if (pids[i] == 0) {  // this is the child process
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i < num_cmds - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            for (int j = 0; j < num_cmds - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            if (!clist->commands[i].argv[0]) {
                fprintf(stderr, "Error: Empty command in pipeline\n");
                exit(ERR_EXEC_CMD);
            }

            if (execvp(clist->commands[i].argv[0], clist->commands[i].argv) == -1) {
                perror("execvp error");
                exit(ERR_EXEC_CMD);
            }
        }
    }

    for (int i = 0; i < num_cmds - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    int status;
    for (int i = 0; i < num_cmds; i++) {
        waitpid(pids[i], &status, 0);
    }

    fflush(stdout);
    return OK;
}
