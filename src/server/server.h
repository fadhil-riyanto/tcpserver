#ifndef _SERVER_H_

#include <netinet/in.h>

struct tcp_structure 
{
    int tcpfd;
    struct sockaddr_in sockaddr_in;
};

void tcpfd_create(struct tcp_structure *tcp_structure);
void tcpfd_set_prop(struct tcp_structure *tcp_structure, char *addr, int listenport);
void tcpfd_bind_and_listen(struct tcp_structure *tcp_structure);

#endif