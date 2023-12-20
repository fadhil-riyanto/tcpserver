#ifndef MAIN_HANDLER_RECV
#define MAIN_HANDLER_RECV

#include <netinet/in.h>

struct conn_prop
{
    int fd;                             // fd from accept
    struct sockaddr_in *sockaddr_in;    // sockaddr_in from accept
};

void conn_to_handle(int fd);

#endif