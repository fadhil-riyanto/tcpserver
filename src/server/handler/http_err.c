#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "header/main_handler_recv.h"

void http_too_many_requests(int fd_for_write)
{
    // dump
    char buf[8192];
    read(fd_for_write, buf, sizeof(buf));
    

    char tempbuf[16384];
    snprintf(tempbuf, sizeof(tempbuf), "HTTP/1.1 429\r\n" 
                    "Connection: closed\r\n"
                    "Content-Type: text/html\r\n\r\n"

                    "<center><h1>429 Too Many Requests</h1></center>"
                    "<br><br>"
                    "request detail: %s"
                    "<hr>"
                    "<center>made by integral2@fadhil_riyanto ran on gdb at pid %d with handler func conn_to_handle func\n", buf, getpid());

    write(fd_for_write, tempbuf, strlen(tempbuf));
}