#include "t_http_parser.h"
#include <stdlib.h>

int main()
{
    static char http_payload_sample[8192] =
        "GET /data?wkwk=abc%20def%20ghi HTTP/1.1\r\n"        // start from 0 to 39 + two carriage return, so
        "Host: 127.0.0.1:10001\r\n"                          // next, (39 + 2) = 41, start 42 to (42 + 21),
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:120.0) Gecko/20100101 Firefox/120.0\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\n"
        "Accept-Language: en-US,en;q=0.5\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Connection: keep-alive\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "Sec-Fetch-Dest: document\r\n"
        "Sec-Fetch-Mode: navigate\r\n"
        "Sec-Fetch-Site: none\r\n"
        "Sec-Fetch-Site: none\r\n"
        "Sec-Fetch-soyu: none\r\n"
        "Sec-Fetch-alpha: none\r\n"
        "Sec-Fetch-betha: none\r\n"
        "Sec-Fetch-User: ?1\r\n";

    struct http_parse_result http_parse_result;

    

    struct parse_prop_internal_ parse_prop_internal_ = http_parse_loads(http_payload_sample, sizeof(http_payload_sample));
    http_parse_start(&parse_prop_internal_, &http_parse_result);

    // second test
    // char *sample = "abcdefghij";
    // char *data = return_string_by_index(sample, 2, 5); // cde

    // http method string (optional)
    const char* HTTP_METHODS_STR[] = {
        "HTTP_GET", "HTTP PUT", "HTTP POST" 
    };

    const char* HTTP_VERSION_STR[] = {
        "HTTP_0.9", "HTTP 1.0", "HTTP 1.1" 
    };

    printf("result: \n\n"
                    "http method    :%s\n"
                    "http route     :%s\n"
                    "http version   :%s\n"
                    "host           :%s\n", HTTP_METHODS_STR[http_parse_result.method], http_parse_result.URI,
                                            HTTP_VERSION_STR[http_parse_result.version], http_parse_result.host);

    // printf("%d\n", http_parse_result.version);
    // free(data);
    // char *sample = "abcfwfghij";
    // char *data = return_string_by_index(sample, 7, 10); // hij

    // printf("result: \"%s\"\n", data);

    // free(data);
}