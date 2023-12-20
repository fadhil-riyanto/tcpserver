#include "header/main_handler_recv.h"
#include <stdio.h>

void conn_accept(int fd, struct conn_prop *conn_prop)
{
    int accept_ret;
    char buffer[4096];
    socklen_t socklen;
    struct sockaddr_in sockaddr_in;
    

    socklen = sizeof(sockaddr_in);
        
    accept_ret = accept(fd, (struct sockaddr*)&sockaddr_in, &socklen);

    

    char buf[4096];

    read(accept_ret, buf, sizeof(buf));

    
    char tempbuf[4096];
    snprintf(tempbuf, sizeof(tempbuf), "HTTP/1.1 200\r\n" 
                    "Connection: closed\r\n"
                    "Content-Type: text/html\r\n\r\n"
                    "it works!, http parse test <h1>test</h1>"
                    "<br>"
                    "<h5>your request payload</h5>%s"
                    "<hr>"
                    "<center>made by ./tcpserver gdb debugging at pid %d", buf, getpid());
    send(accept_ret, tempbuf, sizeof(tempbuf) - 1, 0);
}

void conn_to_handle(int fd)
{
    struct conn_prop conn_prop;
    conn_accept(fd, &conn_prop);
       
}