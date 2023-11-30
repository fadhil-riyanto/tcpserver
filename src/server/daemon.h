#ifndef _DAEMON_H_
#define _DAEMON_H_

struct initialization_space
{
    // main
    /*
    * tcp_structure: carrier from main thread, where tcp FD are returned
    * epoll_prop: carrier from epoll main thread FD;
    */

    struct tcp_structure **tcp_structure;
    struct epoll_prop **epoll_prop;
};

int start_daemon(struct tcp_structure *tcp_structure);

#endif