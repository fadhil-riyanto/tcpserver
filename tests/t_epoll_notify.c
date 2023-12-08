#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int ret, bytesread;
    char buf[100];
    // watch stdin

    struct epoll_event epoll_event = {
        EPOLLOUT, .data.fd = 2
    };
    struct epoll_event epoll_event1 = {
        EPOLLOUT, .data.fd = 1
    };

    struct epoll_event epoll_events[10];

    int epfd;
    epfd = epoll_create(2);
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, 2, &epoll_event) <= -1)
    {
        perror("epoll");
        close(epfd);
    }
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, 1, &epoll_event1) <= -1)
    {
        perror("epoll");
        close(epfd);
    }

    while(1)
    {
        ret = epoll_wait(epfd, epoll_events, 10, -1);
        for (int i = 0; i < ret; i++) {
            memset(buf, 0, sizeof(buf));
            bytesread = read(epoll_events[i].data.fd, buf, sizeof(buf));
            buf[bytesread] = '\0';

            printf("recv: fd %d \"%s\"", epoll_events[i].data.fd, buf);
        }
    }

}