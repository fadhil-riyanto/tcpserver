#include "daemon.h"
#include "../helper/header/epollfn.h"
#include "server.h"
#include "handler/header/main_handler_recv.h"

#include <bits/pthreadtypes.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <pthread.h>
#include <signal.h>

int volatile signal_status = 0;

struct container_data2thread
{
    // use to select index of this one
    int free_num; // number recv from creating a child
    struct multithreading_struct *multithreading_struct; // for cleaning session utils

    int fd_num_unique; // number from epoll
    struct epoll_prop *epoll_prop; // to select epoll
};


void signal_handler(int revc_signum)
{
    signal_status = revc_signum;
}
/*
 * signum: watching signal
 * 
 */
void setup_signal(int signum)
{
    signal(signum, signal_handler);
}


void doing_cleaning()
{

}

void init_child_workers(struct multithreading_struct *multithreading_struct)
{
    for(int i = 0; i < _CONFIG_TCP_MAX_CONN; i++)
    {
        multithreading_struct[i].state = DEAD;
        multithreading_struct[i].ready_to_be_use = YES; 
        multithreading_struct[i].timestamp = 0;
    }
}

/* algorithm
 * 
 * (state eq DEAD) and (ready eq YES) and (now < (timestamp + 180))
 */
int rand_thread_get_free_num(struct multithreading_struct *multithreading_struct)
{
    for(int i = 0; i < _CONFIG_TCP_MAX_CONN; i++)
    {
        if (multithreading_struct[i].state == DEAD && multithreading_struct[i].ready_to_be_use == YES && multithreading_struct[i].timestamp == 0)
        {
            return i;
        }
    }
    return -1; // no thread avaiable
}

// note
// needed: fd (reply)
//           
void *thread_runner(void *data)
{
    struct container_data2thread *container_data2thread = (struct container_data2thread*)data;
    conn_to_handle(container_data2thread->epoll_prop->events[container_data2thread->fd_num_unique].data.fd);

    
    close(container_data2thread->multithreading_struct[container_data2thread->free_num].
        fd_from_accept);

    return 0;
}

int make_child(struct multithreading_struct *multithreading_struct, 
        struct epoll_prop *epoll_prop, int fd_num_unique, pthread_t *pthread_id)
{
    int free_num, accept_ret;
    char buffer[4096];
    socklen_t socklen;

    struct sockaddr_in sockaddr_in;
    
    free_num = rand_thread_get_free_num(multithreading_struct);
    if (free_num < 0) 
    {
        // handle no enuch child here
        // close(ret by handler fd)
        socklen = sizeof(sockaddr_in);
        accept_ret = accept(epoll_prop->events[fd_num_unique].data.fd, (struct sockaddr*)&sockaddr_in, &socklen);
        
        char tempbuf[23] = "server currently busy\n";
        write(accept_ret, tempbuf, sizeof(tempbuf) - 1);

        printf("recvfrom %d", free_num);
        close(accept_ret);
    } else {
        

        // insert
        // multithreading_struct[free_num].fd_from_accept = 0;
        multithreading_struct[free_num].state = ALIVE;
        multithreading_struct[free_num].ready_to_be_use = NO;
        // multithreading_struct[free_num].sockaddr_in = sockaddr_in;
        multithreading_struct[free_num].thread_addr = pthread_id;
        multithreading_struct[free_num].timestamp = time(NULL);

        // joining the data
        struct container_data2thread container_data2thread;
        container_data2thread.free_num = free_num;
        container_data2thread.multithreading_struct = multithreading_struct;
        container_data2thread.fd_num_unique = fd_num_unique;
        container_data2thread.epoll_prop = epoll_prop;

        // char tempbuf[23] = "server works\n";
        // write(accept_ret, tempbuf, sizeof(tempbuf));
        pthread_create(multithreading_struct[free_num].thread_addr, NULL, thread_runner, (void*)&container_data2thread);
        
        // char tempbuf[5] = "hai\n";
        // write(accept_ret, tempbuf, sizeof(tempbuf));

        // printf("recvfrom %d", free_num);
        // close(accept_ret);
    }



    // logic section
    //close(multithreading_struct_sess->unique_fd_num);

    return 0;
}

int start_daemon(struct tcp_structure *tcp_structure)
{
    // set signal
    setup_signal(SIGINT);

    int ret, read_ret, parent_accept_fd, fd_ready_by_epoll = 0;
    char buf[4096];
    int resultread;

    struct sockaddr_in incoming_connection;
    struct epoll_prop epoll_prop;
    struct multithreading_struct multithreading_struct[_CONFIG_TCP_MAX_CONN];
    pthread_t pthread_id[_CONFIG_TCP_MAX_CONN];
    
    socklen_t addrlen;

    // make sure you have enugh child
    init_child_workers(multithreading_struct);

    epoll_init(&epoll_prop);
    epoll_set_watch(&epoll_prop, EPOLLIN, tcp_structure->tcpfd);
    addrlen = sizeof(incoming_connection);
    // ret = accept(tcp_structure->tcpfd, (struct sockaddr*)&incoming_connection, &addrlen);

    // printf("%d", errno);
    // char buf[1024];
    while(1)
    {
        fd_ready_by_epoll = epoll_wait(epoll_prop.epfd, epoll_prop.events, _CONFIG_TCP_MAX_CONN, -1);
        printf("%d nums epoll %d\n", fd_ready_by_epoll, errno);
        // break;
        for (int i = 0; i < fd_ready_by_epoll; i++) {
            // printf("data is 0x%X04 \n", epoll_prop.events[i].events);
            // //printf("eventnum %d\n", epoll_prop.events[i].events);

            if ((epoll_prop.events[i].events & EPOLLERR) || 
                (epoll_prop.events[i].events & EPOLLHUP) || 
                !(epoll_prop.events[i].events & EPOLLIN)) 
            {
                close(epoll_prop.events[i].data.fd);
            } else {
                printf("new conn detected by epollnums %d with fd is %d status %x, fdtotal ret %d \n", i, epoll_prop.events[i].data.fd, epoll_prop.events[i].events, fd_ready_by_epoll);
                make_child(multithreading_struct, &epoll_prop, i, pthread_id);
                // ret = accept(epoll_prop.events[i].data.fd, (struct sockaddr*)&incoming_connection, &addrlen);
                // sleep(2);
                // close(ret);

            }
        }

        
    }

    close(ret);
    close(tcp_structure->tcpfd);
    return 0;
}