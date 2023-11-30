#include "header/epollfn.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int epoll_init(struct epoll_prop *epoll_prop)
{
    epoll_prop->epfd = epoll_create1(0);
    if (epoll_prop->epfd == -1) {
        perror("epoll");
        return -1;
    }
    return epoll_prop->epfd;
}

int epoll_set_watch(struct epoll_prop *epoll_prop, uint32_t epoll_event_type, int fd)
{
    struct epoll_event events[100];
    epoll_prop->events = events;

    struct epoll_event event;
    epoll_prop->epoll_event = &event;
    epoll_prop->epoll_event->events = EPOLLIN;
    epoll_prop->epoll_event->data.fd = fd;

    if (epoll_ctl(epoll_prop->epfd, EPOLL_CTL_ADD, fd, epoll_prop->epoll_event) == -1) {
        perror("epoll_ctl() error");
        close(epoll_prop->epfd);
        return errno;
    }
    return 0;

}