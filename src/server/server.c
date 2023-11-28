#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "server.h"
#include "../config.c"


void tcpfd_create(struct tcp_structure *tcp_structure)
{
    tcp_structure->tcpfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_structure->tcpfd < 0) {
        perror("socket");
    }
}

void tcpfd_set_prop(struct tcp_structure *tcp_structure, char *addr, int listenport)
{
    memset(&tcp_structure->sockaddr_in, 0, sizeof(struct sockaddr_in));

    tcp_structure->sockaddr_in.sin_family = AF_INET;
    tcp_structure->sockaddr_in.sin_port = htons(listenport);
    tcp_structure->sockaddr_in.sin_addr.s_addr = inet_addr(addr);
}


void tcpfd_bind_and_listen(struct tcp_structure *tcp_structure)
{
    int ret;

    ret = bind(tcp_structure->tcpfd, (const struct sockaddr*)&tcp_structure->sockaddr_in, 
            sizeof(tcp_structure->sockaddr_in));
    if (ret != 0) {
        perror("bind");
        close(tcp_structure->tcpfd);
    }

    ret = listen(tcp_structure->tcpfd, TCP_MAX_CONN);
}