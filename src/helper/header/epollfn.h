#ifndef _EPOLLFN_H_
#define _EPOLLFN_H_

#include <stdint.h>

struct epoll_prop
{
    int fd; // fd to watch
    int epfd;
    struct epoll_event *epoll_event;
};

int epoll_init(struct epoll_prop *epoll_prop);
int epoll_set_watch(struct epoll_prop *epoll_prop, uint32_t epoll_event_type, int fd);

#endif