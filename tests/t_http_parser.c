#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "t_http_parser.h"
#include "t_utils.h"

int volatile linecounter = 0;

struct parse_prop_internal_ http_parse_loads(char *data, size_t size)
{
    struct parse_prop_internal_ parse_prop_internal_ = {
        data, 0, size
    };

    return parse_prop_internal_;
}

void internal_parser(char *data, struct http_parse_result *http_parse_result)
{
    char dest[200];
    char firstwords[200];
    int ret, retq;
    int index = 0;

    do {
        // ret = split(' ', data, strlen(data), index, dest);
        ret = split(' ', data, strlen(data), index, dest);
        retq = split(' ', data, strlen(data), 0, firstwords);

        // ret = 3;
        if (linecounter == 0) {
            if (strcmp(firstwords, "GET") == 0) {
                // printf("http get here\n");
                http_parse_result->method = HTTP_GET;
            } else if (strcmp(firstwords, "PUT") == 0) {
                http_parse_result->method = HTTP_PUT;
            } else if (strcmp(firstwords, "POST") == 0) {
                http_parse_result->method = HTTP_POST;
            }

            if (index == 1) {
                strcpy(http_parse_result->URI, dest);
                // printf("%s\n", dest);
            }

            if (index == 3) {
                if (strcmp(dest, "HTTP/0.9") == 0) {
                    http_parse_result->version = HTTP_0_9;
                } else if (strcmp(dest, "HTTP/1.0") == 0) {
                    http_parse_result->version = HTTP_1_0;
                    
                } else if (strcmp(dest, "HTTP/1.1") == 0) {
                    http_parse_result->version = HTTP_1_1;
                } 
                //printf("%d\n", strcmp(data, "GET / HTTP/1.1"));
                //printf("\"%s\n", data);

            }
        } else {
            ret = split(':', data, strlen(data), 0, firstwords);
        }

        // free(dest);
        // free(firstwords);

        index = index + 1;
    } while (ret != 3);

    linecounter = linecounter + 1;
    // get http
    
}

void http_parse_start(struct parse_prop_internal_ *parse_prop_internal_, struct http_parse_result *http_parse_result)
{
    struct explode_map explode_map = {
        0, malloc(0)
    };

    // mapping all offset that contains \r\n
    explode_by_lineend(parse_prop_internal_, &explode_map);

    for(int i = 0; i < explode_map.counting_arr; i++)
    {
        char *string = return_string_by_index(parse_prop_internal_->source, explode_map.explode_mapping_offset[i].starting_from, explode_map.explode_mapping_offset[i].ending_at);
        // printf("%c\n", string[0]);
        internal_parser(string, http_parse_result);


        free(string);
        // printf("start %d end %d\n", explode_map.explode_mapping_offset[i].starting_from, explode_map.explode_mapping_offset[i].ending_at);
    }

    // cleanup
    free(explode_map.explode_mapping_offset);
}

char *return_string_by_index(char *src, int from, int to)
{
    char *returned_string = (char *)malloc(sizeof(char) * ((to - from) + 1));
    int length = to - from;
    int i;

    for(i = 0; i < (length) ; i++)
    {
        returned_string[i] = src[from];
        from = from + 1;
    }

    returned_string[i] = '\0';
    return returned_string;
}

int explode_by_lineend(struct parse_prop_internal_ *parse_prop_internal_, struct explode_map *explode_map)
{
    // init
    int first_literations = 1;
    int i = 0;
    for(;;)
    {
        if(parse_prop_internal_->source[i] == '\r' && \
            parse_prop_internal_->source[(i + 1)] == '\n') 
        {
            explode_map->counting_arr = explode_map->counting_arr + 1;
            explode_map->explode_mapping_offset = realloc(explode_map->explode_mapping_offset, 
                sizeof(struct explode_mapping_offset) * explode_map->counting_arr);

            if (first_literations == 1)
            {
                explode_map->explode_mapping_offset[explode_map->counting_arr - 1].starting_from = 0;
                explode_map->explode_mapping_offset[explode_map->counting_arr - 1].ending_at = (i);

                // change state
                first_literations = 0;
            } else {
                explode_map->explode_mapping_offset[explode_map->counting_arr - 1].starting_from = 
                    (explode_map->explode_mapping_offset[explode_map->counting_arr - 1 - 1].ending_at + 1);
                    
                explode_map->explode_mapping_offset[explode_map->counting_arr - 1].ending_at = (i);
            }
            //printf("found\n");
        } else if (parse_prop_internal_->source[i] == '\0')
        {
            break;
        }
        i++;
    }
    // printf("%d\n", parse_prop_internal_->stop_offset);
}