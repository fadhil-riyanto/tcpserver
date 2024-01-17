#include "daemon.h"
#include "../helper/header/epollfn.h"
#include "server.h"
#include "handler/header/main_handler_recv.h"
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
#include "../helper/header/test_function.h"

int volatile signal_status = 0;
int volatile highest_thread_nums = 0;

struct container_data2thread
{
    // use to select index of this one
    int free_num; // number recv from creating a child
    struct multithreading_struct *multithreading_struct; // for cleaning session utils
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
    int free_num = container_data2thread->free_num;

    //printf("making threadd wkwkw\n");
    
    // int accept_ret;
    char buf[8192];
    // struct sockaddr_in sockaddr_in;
    // socklen_t socklen;
    // socklen = sizeof(sockaddr_in);
    // accept_ret = accept(container_data2thread->tcp_structure->tcpfd, (struct sockaddr*)&sockaddr_in, &socklen);
    
    read(container_data2thread->multithreading_struct[free_num].fd_from_accept, buf, sizeof(buf));

    //printf("%s\n", buf);
    // check_line_ending(buf);
    char tempbuf[16384];
    snprintf(tempbuf, sizeof(tempbuf), "HTTP/1.1 200\r\n" 
                    "Connection: closed\r\n"
                    "Content-Type: text/html\r\n\r\n"
                    "it works!, http parse test <h1>test</h1>"
                    "<br>"
                    "<h5>your request payload</h5>%s"
                    "<hr>"
                    "<center>made by ./tcpserver gdb debugging at pid %d\n", buf, getpid());

    write(container_data2thread->multithreading_struct[free_num].fd_from_accept, tempbuf, strlen(tempbuf));

    
    // printf("buf res %s\n", buf);
    // write(container_data2thread->multithreading_struct[free_num].fd_from_accept, buf, strlen(buf));
    //leep(2);
    close(container_data2thread->multithreading_struct[free_num].fd_from_accept);

    container_data2thread->multithreading_struct[free_num].state = DEAD;
    // char tempbuf[23];
    // snprintf(tempbuf, sizeof(tempbuf), "in else catch %d on thread\n", container_data2thread->free_num);
    // write(accept_ret, tempbuf, sizeof(tempbuf) - 1);
    // close(accept_ret);
    //printf("handling now\n");

    ////##printf("trying handling fds %d\n", container_data2thread->epoll_prop->events[container_data2thread->fd_num_unique].data.fd);
    //conn_to_handle(container_data2thread->epoll_prop->events[container_data2thread->fd_num_unique].data.fd);
    // struct sockaddr_in sockaddr_in;
    // int accept_ret;
    // socklen_t socklen = sizeof(sockaddr_in);
    // accept_ret = accept(container_data2thread->epoll_prop->events[container_data2thread->fd_num_unique].data.fd, (struct sockaddr*)&sockaddr_in, &socklen);
    // char tempbuf[] = "in else catch in thread\n";
    // write(accept_ret, tempbuf, sizeof(tempbuf) - 1);
    // close(accept_ret);
    // //##printf("closing now\n");
    
    // close(container_data2thread->multithreading_struct[container_data2thread->free_num].
    //    fd_from_accept);

    return 0;
}

int make_child(struct multithreading_struct *multithreading_struct, 
        struct tcp_structure *tcp_structure, 
        struct epoll_prop *epoll_prop, int fd_num_unique)
{
    printf("->> making thread parent funcion on make child\n");
    int free_num, accept_ret;
    char buffer[4096];
    socklen_t socklen;

    struct sockaddr_in sockaddr_in;
    
    free_num = rand_thread_get_free_num(multithreading_struct);
    // free_num = -1;
    if (free_num < 0) 
    {
        // handle no enuch child here
        // close(ret by handler fd)
        socklen = sizeof(sockaddr_in);
        accept_ret = accept(epoll_prop->events[fd_num_unique].data.fd, (struct sockaddr*)&sockaddr_in, &socklen);
        
        char tempbuf[23] = "server currently busy\n";
        write(accept_ret, tempbuf, sizeof(tempbuf) - 1);

        printf("recvfrom %d\n", free_num);
        close(accept_ret);
    } else {
        socklen = sizeof(sockaddr_in);
        accept_ret = accept(epoll_prop->events[fd_num_unique].data.fd, (struct sockaddr*)&sockaddr_in, &socklen);


        // insert
        multithreading_struct[free_num].fd_from_accept = accept_ret;
        multithreading_struct[free_num].state = ALIVE;
        multithreading_struct[free_num].ready_to_be_use = NO;
        multithreading_struct[free_num].sockaddr_in = sockaddr_in;
        multithreading_struct[free_num].timestamp = time(NULL);

        // joining the data
        struct container_data2thread container_data2thread;
        container_data2thread.free_num = free_num;
        container_data2thread.multithreading_struct = multithreading_struct;
        // char tempbuf[23] = "server works\n";
        // write(accept_ret, tempbuf, sizeof(tempbuf));
   
        // socklen = sizeof(sockaddr_in);
        // accept_ret = accept(epoll_prop->events[fd_num_unique].data.fd, (struct sockaddr*)&sockaddr_in, &socklen);
        // char tempbuf[23];
        // snprintf(tempbuf, sizeof(tempbuf), "in else catch %d on thread\n", free_num);
        // write(accept_ret, tempbuf, sizeof(tempbuf) - 1);
        // close(accept_ret);
        //return 0;
        
        pthread_create(&multithreading_struct[free_num].thread_addr, NULL, thread_runner, (void*)&container_data2thread);
        // pthread_create(multithreading_struct[free_num].thread_addr, NULL, thread_runner, (void*)&container_data2thread);
        
        // get highest
        if (free_num > highest_thread_nums)
        {
            highest_thread_nums = free_num;
        }
        //##printf("HERE YOUR CREATEDDDDDDDDDDDD %d\n", free_num);
        // char tempbuf[5] = "hai\n";
        // write(accept_ret, tempbuf, sizeof(tempbuf));

        // //##printf("recvfrom %d", free_num);
        // close(accept_ret);
    }



    // logic section
    //close(multithreading_struct_sess->unique_fd_num);

    return 0;
}

struct main_to_2nd_main_stack_data
{
    struct epoll_prop *epoll_prop;
    struct multithreading_struct *multithreading_struct;
    struct tcp_structure *tcp_structure;
};

void* run_eventloop_daemon(void *data)
{
    // unpacking
    struct main_to_2nd_main_stack_data *from_main_thread = (struct main_to_2nd_main_stack_data*)data;

    // init
    int fd_ready_by_epoll = 0;
    int ret;

    // temp
    struct sockaddr_in sockaddr_in;

    while(1)
    {
        //##printf("%d epoll retq\n", from_main_thread->epoll_prop->epfd);
        
        
        fd_ready_by_epoll = epoll_wait(from_main_thread->epoll_prop->epfd, from_main_thread->epoll_prop->events, _CONFIG_TCP_MAX_CONN, -1);
        
        //##printf("%d nums epoll %d\n", fd_ready_by_epoll, errno);
        // break;
        for (int i = 0; i < fd_ready_by_epoll; i++) {
            // //##printf("data is 0x%X04 \n", from_main_thread->epoll_prop->events[i].events);
            //##printf("recv signal is  %d\n", signal_status);
            //printf("loop loop %u\n", from_main_thread->epoll_prop->events[i].events);
            
            if ((from_main_thread->epoll_prop->events[i].events & EPOLLERR) || 
                (from_main_thread->epoll_prop->events[i].events & EPOLLHUP) || 
                !(from_main_thread->epoll_prop->events[i].events & EPOLLIN)) 
            {
                close(from_main_thread->epoll_prop->events[i].data.fd);
            } else {
                //printf("()()() its should be executed 2 times\n");
                if (from_main_thread->epoll_prop->events[i].events & EPOLLIN)
                {
                    printf("()()() epoll IN detected\n");
                }
                // char buf[100];
                // socklen_t addrlen = sizeof(sockaddr_in);
                // ret = accept(from_main_thread->epoll_prop->events[i].data.fd, (struct sockaddr*)&sockaddr_in, &addrlen);
                // // sleep(2);
                // read(ret, buf, sizeof(buf));
                // printf("trying to sending oke\n");
                // write(ret, "oke\n", 4);
                // close(ret);
                //printf("epfdfrom %d and tcpfd %d", from_main_thread->epoll_prop->events[i].data.fd, from_main_thread->tcp_structure->tcpfd);
                // if (from_main_thread->epoll_prop->events[i].data.fd == from_main_thread->tcp_structure->tcpfd)
                // {
                // int accept_fd_temp;
                // accept_fd_temp = accept()
                // printf("MAKING THREAD on eventlooop %d\n", fd_ready_by_epoll);
                
                make_child(from_main_thread->multithreading_struct, from_main_thread->tcp_structure, from_main_thread->epoll_prop, i);
                // }
                //##printf("new conn detected by epollnums %d with fd is %d status %x, fdtotal ret %d \n", i, from_main_thread->epoll_prop->events[i].data.fd, from_main_thread->epoll_prop->events[i].events, fd_ready_by_epoll);
            } 
           

            // }
        }

        
    }
}


void daemon_cleaning(struct multithreading_struct *multithreading_struct, struct tcp_structure *tcp_structure)
{
    for(int i = 0; i < _CONFIG_TCP_MAX_CONN; i++)
    {
        
        if (
            multithreading_struct[i].ready_to_be_use == NO && 
            multithreading_struct[i].state == DEAD && 
            (_CONFIG_TCP_SPACE_BETWEEN_TIME_CLEANUP != 0 
                ? (((multithreading_struct[i].timestamp + _CONFIG_TCP_SPACE_BETWEEN_TIME_CLEANUP) < time(NULL))) 
                : 1 ))
        {
            pthread_join(multithreading_struct[i].thread_addr, NULL);
            multithreading_struct[i].ready_to_be_use = YES;
            //pthread_join(multithreading_struct->thread_addr, NULL);
            printf("thread %d joined\n", i);


        }
    }
}

void cleaning_thread_stack(struct multithreading_struct *multithreading_struct, struct tcp_structure *tcp_structure,
    pthread_t *loop_runnner_pthread_id, struct epoll_prop *epoll_prop)
{
    // cleaning and killing the thread
    for(int i = 0; i < _CONFIG_TCP_MAX_CONN; i++) {
        if (multithreading_struct[i].state == ALIVE) 
        {
            pthread_kill(multithreading_struct[i].thread_addr, SIGINT);
            multithreading_struct[i].state = DEAD;
        }

        if (multithreading_struct[i].ready_to_be_use == NO)
        {
            pthread_join(multithreading_struct[i].thread_addr, NULL);
            multithreading_struct[i].ready_to_be_use = YES;
        }

        // kill main thread
        pthread_cancel(*loop_runnner_pthread_id);
        pthread_kill(*loop_runnner_pthread_id, SIGINT);

        close(tcp_structure->tcpfd);
        close(epoll_prop->epfd);

        printf("closing fd\n");

        exit(0);

        

    }

    
}

void start_idling(struct multithreading_struct *multithreading_struct, struct tcp_structure *tcp_structure,
    pthread_t *loop_runnner_pthread_id, struct epoll_prop *epoll_prop)
{
    int counter = 0;

    while(1)
    {
        switch (signal_status) {
            case 0:
                //##printf("[eventloop-%d] watching, highest %d\n", counter, highest_thread_nums);
                daemon_cleaning(multithreading_struct, tcp_structure);
                counter++;
            break;
            case 2:
                printf("[eventloop-%d] SIGINT, highest %d\n", counter, highest_thread_nums);
                cleaning_thread_stack(multithreading_struct, tcp_structure, loop_runnner_pthread_id, epoll_prop);
                // exit(0);
                // cleaning here
            break;
        }

        sleep(1);
    }
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
    pthread_t loop_runnner_pthread_id;
    
    socklen_t addrlen;

    // make sure you have enugh child
    init_child_workers(multithreading_struct);

    epoll_init(&epoll_prop);
    epoll_set_watch(&epoll_prop, EPOLLIN, tcp_structure->tcpfd);


    struct main_to_2nd_main_stack_data main_to_2nd_main_stack_data = {
        &epoll_prop, multithreading_struct, tcp_structure
    };

    pthread_create(&loop_runnner_pthread_id, NULL, run_eventloop_daemon, (void*)&main_to_2nd_main_stack_data);
    // addrlen = sizeof(incoming_connection);
    // ret = accept(tcp_structure->tcpfd, (struct sockaddr*)&incoming_connection, &addrlen);

    // //##printf("%d", errno);
    // char buf[1024];

    //while(1){ sleep(1); //##printf("looping %d\n", loops); loops++;}
    start_idling(multithreading_struct, tcp_structure, &loop_runnner_pthread_id, &epoll_prop);
    // close(ret);
    // close(tcp_structure->tcpfd);
    return 0;
}
