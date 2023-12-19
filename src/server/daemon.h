#ifndef _DAEMON_H_
#define _DAEMON_H_

#include "server.h"

typedef enum 
{
    ALIVE,
    DEAD
} state_status_t ;

typedef enum 
{
    YES,
    NO
} ready_to_be_use_t;

struct multithreading_struct // will be copied into each connection, and making thread for them
{
    int unique_fd_num; // fd given by epolll
    state_status_t state; // alive or not for tcp conn
    ready_to_be_use_t ready_to_be_use; // means there is was joined by pthread_join() or not
    int *fd_from_accept; // fd recv from accepting
};

int start_daemon(struct tcp_structure *tcp_structure);

#endif