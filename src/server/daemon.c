#include "server.h"
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

void epoll_watch()

int start_daemon()
{
    int ret;
    int parent_accept_fd;

    socklen_t addrlen;

    struct tcp_structure tcp;
    struct sockaddr_in incoming_connection;

    tcpfd_create(&tcp);
    tcpfd_set_prop(&tcp, NULL, 8086);
    tcpfd_bind_and_listen(&tcp);

    addrlen = sizeof(incoming_connection);
    ret = accept(tcp.tcpfd, (struct sockaddr*)&incoming_connection, &addrlen);

    printf("ret return %d errnum is %d\n", ret, errno);


    char buf[1024];
    for(;;)
    {
        bzero(&buf, 1024);
        read_ret = read(ret, buf, sizeof(buf)); 
        // if (strcmp(buf, "") != 0)
        // {
        //     printf("recv %s\n", buf);
        // }
        buf[read_ret - 1] = '\0';

        printf("recv %s\n", buf);
        sleep(1);
    }

    close(ret);
    close(tcp.tcpfd);
}