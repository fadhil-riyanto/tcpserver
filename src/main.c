#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper/header/cmd_parser.h"
#include "server/server.h"
#include "server/daemon.h"

void showhelp()
{
    char *text = "please use \n\n"\
            "./server --addr 127.0.0.1 --port 9\n\n" \
            "where:\n" \
            "--addr is your listen address\n" \
            "--port your opened port for connection\n";

    printf("%s", text);
            
}
void _main(struct cmd_prop *cmd_prop)
{
    struct tcp_structure tcp_structure;
    struct sockaddr_in incoming_connection;

    tcpfd_create(&tcp_structure);
    tcpfd_set_prop(&tcp_structure, cmd_prop->server_addr, cmd_prop->port);
    tcpfd_bind_and_listen(&tcp_structure);
    // printf("%d", errno);
    // exit(1);
    start_daemon(&tcp_structure);
}


int main(int argc, char *argv[])
{
    struct cmd_prop cmd_prop;
    optparse(argc, argv, &cmd_prop);

    if (cmd_prop.cmd_prop_status.server_addr == 1 && cmd_prop.cmd_prop_status.port == 1)
    {
        _main(&cmd_prop);
    } else {
        showhelp();
    }

    //printf("%d\n", cmd_prop.port);

    // int ret, read_ret;
    // socklen_t addrlen;
    // struct tcp_structure tcp;
    // struct sockaddr_in input;
    // printf("bekum create here\n");
    // tcpfd_create(&tcp);
    // printf("unlisten here\n");
    // tcpfd_set_prop(&tcp, NULL, 8086);
    
    // tcpfd_bind_and_listen(&tcp);
    // printf("stucj here\n");

    // addrlen = sizeof(input);
    // ret = accept(tcp.tcpfd, (struct sockaddr*)&input, &addrlen);

    // printf("ret return %d errnum is %d\n", ret, errno);


    // char buf[1024];
    // for(;;)
    // {
    //     bzero(&buf, 1024);
    //     read_ret = read(ret, buf, sizeof(buf)); 
    //     // if (strcmp(buf, "") != 0)
    //     // {
    //     //     printf("recv %s\n", buf);
    //     // }
    //     buf[read_ret - 1] = '\0';

    //     printf("recv %s\n", buf);
    //     sleep(1);
    // }

    // close(ret);
    // close(tcp.tcpfd);
}