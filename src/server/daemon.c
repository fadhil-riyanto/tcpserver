#include "daemon.h"
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include "../helper/header/epollfn.h"
#include <sys/epoll.h>
#include <netinet/in.h>
#include "server.h"
#include <stdlib.h>



void daemon_initialization_space(struct initialization_space *init,
        struct tcp_structure **tcp_structure, struct epoll_prop **epoll_prop)
{
    init->tcp_structure = tcp_structure;
    init->epoll_prop = epoll_prop;
}

int start_daemon(struct tcp_structure *tcp_structure)
{
    int ret, read_ret, parent_accept_fd, fd_ready_by_epoll = 0;

    struct sockaddr_in incoming_connection;
    struct epoll_prop epoll_prop;
    
    socklen_t addrlen;

    epoll_init(&epoll_prop);
    epoll_set_watch(&epoll_prop, EPOLLIN, tcp_structure->tcpfd);
    addrlen = sizeof(incoming_connection);
    // ret = accept(tcp_structure->tcpfd, (struct sockaddr*)&incoming_connection, &addrlen);

    // printf("%d", errno);
    // char buf[1024];
    for(;;)
    {
        fd_ready_by_epoll = epoll_wait(epoll_prop.epfd, epoll_prop.events, _CONFIG_EPOLL_MAX_EVENTS, -1);

        
        if (fd_ready_by_epoll != 0)
        {
            printf("recv %d\n", fd_ready_by_epoll);
            printf("errnum %d", errno);

            close(epoll_prop.epfd);
            close(tcp_structure->tcpfd);
            exit(1);
        }
        // bzero(&buf, 1024);
        // read_ret = read(ret, buf, sizeof(buf)); 
        // // if (strcmp(buf, "") != 0)
        // // {
        // //     printf("recv %s\n", buf);
        // // }
        // buf[read_ret - 1] = '\0';

        printf("recv %d\n", fd_ready_by_epoll);
        // sleep(1);
        // if (strcmp(buf, "bye") == 0) {
        //     break;
        // }
    }

    // close(ret);
    // close(tcp_structure->tcpfd);
    return 0;
}