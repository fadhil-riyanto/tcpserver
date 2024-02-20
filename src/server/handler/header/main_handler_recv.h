#ifndef MAIN_HANDLER_RECV
#define MAIN_HANDLER_RECV

#include <netinet/in.h>

struct conn_prop
{
    int fd_for_write;                             // fd from accept, used to write operation (output)
    struct sockaddr_in *sockaddr_in;            // sockaddr_in from accept

    char *datarecv_from_fd;
};

void conn_to_handle(int fd_for_write, struct sockaddr_in *sockaddr_in, char *datarecv_from_fd);

#endif