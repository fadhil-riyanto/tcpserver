#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "server.h"

struct multithreading_struct // will be copied into each connection, and making thread for them
{
    int unique_fd_num; // fd given by epolll
    int state; // alive or not for tcp conn
    int ready_to_be_use; // means there is was joined by pthread_join() or not
    int *fd_from_accept; // fd recv from accepting
};

int start_daemon(struct tcp_structure *tcp_structure);

#endif