#ifndef HTTP_PARSER_H_
#define HTTP_PARSER_H_

#include <stdio.h>
#include <stdlib.h>



// config
#define HTTP_MAX_URI 4000

// types
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

// internal
struct exploded_string_by_lineend
{
    char *string;
};

struct explode_mapping_offset
{
    int starting_from;
    int ending_at;
};

struct explode_map
{
    int counting_arr;
    struct explode_mapping_offset *explode_mapping_offset;
};

// functions!
// main menu
struct parse_prop_internal_ http_parse_loads(char *data, size_t size);
void http_parse_start(struct parse_prop_internal_ *parse_prop_internal_, struct http_parse_result *http_parse_result);

// internal
char *return_string_by_index(char *src, int from, int to);
int explode_by_lineend(struct parse_prop_internal_ *parse_prop_internal_, struct explode_map *explode_map);

#endif