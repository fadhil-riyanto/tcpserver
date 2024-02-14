#include "header/main_handler_recv.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void conn_to_handle(int fd_for_write, struct sockaddr_in *sockaddr_in, char *datarecv_from_fd)
{
    char tempbuf[16384];
    snprintf(tempbuf, sizeof(tempbuf), "HTTP/1.1 200\r\n" 
                    "Connection: closed\r\n"
                    "Content-Type: text/html\r\n\r\n"
                    "it works!, http parse test <h1>test</h1>"
                    "<br>"
                    "<h5>your request payload</h5>%s"
                    "<hr>"
                    "<center>made by ./tcpserver gdb debugging at pid %d on conn_to_handle func\n", datarecv_from_fd, getpid());

    write(fd_for_write, tempbuf, strlen(tempbuf));

}