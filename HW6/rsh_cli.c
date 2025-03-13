#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dshlib.h"
#include "rshlib.h"

int exec_remote_cmd_loop(char *address, int port) {
    int cli_socket = start_client(address, port);
    if (cli_socket < 0) {
        perror("start client");
        return ERR_RDSH_CLIENT;
    }

    char *cmd_buff = malloc(RDSH_COMM_BUFF_SZ);
    char *rsp_buff = malloc(RDSH_COMM_BUFF_SZ);
    if (!cmd_buff || !rsp_buff) {
        return client_cleanup(cli_socket, cmd_buff, rsp_buff, ERR_MEMORY);
    }

    while (1) {
        printf("%s", SH_PROMPT);
        fflush(stdout);

        if (!fgets(cmd_buff, RDSH_COMM_BUFF_SZ, stdin)) break;
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (send(cli_socket, cmd_buff, strlen(cmd_buff) + 1, 0) < 0) {
            perror("send");
            break;
        }

        ssize_t recv_bytes;
        while ((recv_bytes = recv(cli_socket, rsp_buff, RDSH_COMM_BUFF_SZ, 0)) > 0) {
            if (rsp_buff[recv_bytes - 1] == RDSH_EOF_CHAR) {
                rsp_buff[recv_bytes - 1] = '\0';
                printf("%s\n", rsp_buff);  // prints command output from server
                break;
            }
            printf("%.*s", (int)recv_bytes, rsp_buff);
        }

        if (strcmp(cmd_buff, "exit") == 0) break;
    }

    return client_cleanup(cli_socket, cmd_buff, rsp_buff, OK);
}

int start_client(char *server_ip, int port) {
    struct sockaddr_in addr;
    int cli_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_socket < 0) {
        perror("socket");
        return ERR_RDSH_CLIENT;
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(server_ip);
    addr.sin_port = htons(port);

    if (connect(cli_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(cli_socket);
        return ERR_RDSH_CLIENT;
    }

    return cli_socket;
}

int client_cleanup(int cli_socket, char *cmd_buff, char *rsp_buff, int rc) {
    if (cli_socket > 0) close(cli_socket);
    free(cmd_buff);
    free(rsp_buff);
    return rc;
}
