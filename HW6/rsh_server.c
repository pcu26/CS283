#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "dshlib.h"
#include "rshlib.h"

// prevent implicit declaration error
void execute_remote_command(int cli_socket, char *command);

int start_server(char *ifaces, int port, int is_threaded) {
    (void) is_threaded; 
    int svr_socket;
    int rc;

    svr_socket = boot_server(ifaces, port);
    if (svr_socket < 0) {
        return svr_socket;
    }

    rc = process_cli_requests(svr_socket);
    stop_server(svr_socket);
    return rc;
}

int stop_server(int svr_socket) {
    return close(svr_socket);
}

int boot_server(char *ifaces, int port) {
    int svr_socket;
    struct sockaddr_in addr;

    svr_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (svr_socket < 0) {
        perror("socket");
        return ERR_RDSH_COMMUNICATION;
    }

    int enable = 1;
    setsockopt(svr_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ifaces);
    addr.sin_port = htons(port);

    if (bind(svr_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(svr_socket);
        return ERR_RDSH_COMMUNICATION;
    }

    if (listen(svr_socket, 20) == -1) {
        perror("listen");
        close(svr_socket);
        return ERR_RDSH_COMMUNICATION;
    }

    return svr_socket;
}

int process_cli_requests(int svr_socket) {
    int cli_socket;
    int rc = OK;

    while (1) {
        cli_socket = accept(svr_socket, NULL, NULL);
        if (cli_socket < 0) {
            perror("accept");
            return ERR_RDSH_COMMUNICATION;
        }

        rc = exec_client_requests(cli_socket);
        close(cli_socket);

        if (rc == OK_EXIT) break;
    }

    return rc;
}

int exec_client_requests(int cli_socket) {
    char *io_buff = malloc(RDSH_COMM_BUFF_SZ);
    if (!io_buff) return ERR_RDSH_SERVER;

    while (1) {
        ssize_t recv_bytes = recv(cli_socket, io_buff, RDSH_COMM_BUFF_SZ, 0);
        if (recv_bytes <= 0) break;

        io_buff[recv_bytes] = '\0';
        printf("Received: %s\n", io_buff);

        if (strcmp(io_buff, "exit") == 0) break;
        if (strcmp(io_buff, "stop-server") == 0) {
            free(io_buff);
            return OK_EXIT;
        }

        // execute received command
        execute_remote_command(cli_socket, io_buff);
    }

    free(io_buff);
    return OK;
}

// function that runs commands and sends output to client
void execute_remote_command(int cli_socket, char *command) {
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) { // child process
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        char *argv[CMD_ARGV_MAX];
        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL && i < CMD_ARGV_MAX - 1) {
            argv[i++] = token;
            token = strtok(NULL, " ");
        }
        argv[i] = NULL;

        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
    } else { // parent process
        close(pipefd[1]);

        char buffer[RDSH_COMM_BUFF_SZ];
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytes_read] = '\0';
            send(cli_socket, buffer, bytes_read, 0);
        }
        close(pipefd[0]);

        waitpid(pid, NULL, 0);
        send_message_eof(cli_socket);  
    }
}


int send_message_eof(int cli_socket) {
    int sent_len = send(cli_socket, &RDSH_EOF_CHAR, 1, 0);
    return (sent_len == 1) ? OK : ERR_RDSH_COMMUNICATION;
}
