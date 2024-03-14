#include "header/main_handler_recv.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../../helper/header/http_parser.h"
#include "../../helper/header/uriparser.h"

void conn_to_handle(int fd_for_write, struct sockaddr_in *sockaddr_in, char *datarecv_from_fd)
{
    // printf("start to parsing %s\n", datarecv_from_fd);
    struct http_parse_result http_parse_result;
    struct parse_prop_internal_ parse_prop_internal_ = http_parse_loads(datarecv_from_fd, sizeof(datarecv_from_fd));
    http_parse_start(&parse_prop_internal_, &http_parse_result);

    //char *texttest = "/tests?data=blah&wkwk=kkk";
    struct internal_dataparser internal_dataparser;
    urlparser_parse(&internal_dataparser, http_parse_result.URI, strlen(http_parse_result.URI));

    // printf("dataku %s:\n", urlparser_stringfy(&internal_dataparser));
    char tempbuf[16384];
    snprintf(tempbuf, sizeof(tempbuf), "HTTP/1.1 200\r\n" 
                    "Connection: closed\r\n"
                    "Content-Type: text/html\r\n\r\n"
                    "<h1>it works!</h1>, http parse test"
                    "<br>"
                    "<h5>your request path</h5> : %s"
                    "<br><br><br>http parameter: %s"
                    "<hr>"
                    "<center>made by integral2@fadhil_riyanto. run on gdb at pid %d with handler func conn_to_handle\n", http_parse_result.URI, urlparser_stringfy(&internal_dataparser), getpid());

    write(fd_for_write, tempbuf, strlen(tempbuf));

}