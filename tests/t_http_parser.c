#include <stddef.h>

// tests
#include <stdio.h>

// config
#define HTTP_MAX_URI 4000

typedef enum 
{
    HTTP_GET, HTTP_PUT, HTTP_POST
} HTTP_METHODS_t;

typedef enum 
{
    HTTP_0_9, HTTP_1_0, HTTP_1_1
} HTTP_VERSION_t;

struct parse_prop_internal_
{
    char *source;
    int stop_offset;
    size_t size;
};

struct host_prop_internal_
{
    char host[253];
    int post;
};

struct http_parse_result
{
    HTTP_METHODS_t method;
    char URI[HTTP_MAX_URI];
    struct host_prop_internal_ host;
    char *useragent;

};

struct exploded_string_by_lineend
{
    char *string;
};

static struct parse_prop_internal_ http_parse_loads(char *data, size_t size)
{
    struct parse_prop_internal_ parse_prop_internal_ = {
        data, 0, size
    };

    return parse_prop_internal_;
}

static int explode_by_lineend(struct parse_prop_internal_ *parse_prop_internal_)
{

    // init
    
    int i = 0;
    for(;;)
    {
        if(parse_prop_internal_->source[parse_prop_internal_->stop_offset + i] == '\r' && \
            parse_prop_internal_->source[parse_prop_internal_->stop_offset + (i + 1)] == '\n')
        {
            
            //parse_prop_internal_->stop_offset = parse_prop_internal_->stop_offset + 1;
            printf("found at %d and %d", i, i + 1);
            break;
        }
        i++;
    }
    // printf("%d\n", parse_prop_internal_->stop_offset);
}


int main()
{
    static char http_payload_sample[8192] =
        "GET / HTTP/1.1\r\n"
        "Host: 127.0.0.1:10001\r\n"
        "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:120.0) Gecko/20100101 Firefox/120.0\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\n"
        "Accept-Language: en-US,en;q=0.5\r\n"
        "Accept-Encoding: gzip, deflate, br\r\n"
        "Connection: keep-alive\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "Sec-Fetch-Dest: document\r\n"
        "Sec-Fetch-Mode: navigate\r\n"
        "Sec-Fetch-Site: none\r\n"
        "Sec-Fetch-User: ?1\r\n";

    struct parse_prop_internal_ parse_prop_internal_ = http_parse_loads(http_payload_sample, sizeof(http_payload_sample));
    explode_by_lineend(&parse_prop_internal_);
    // printf("%s\n", parse_prop_internal_.source);
}