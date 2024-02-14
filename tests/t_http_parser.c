#include <stddef.h>
#include "t_http_parser.h"

struct parse_prop_internal_ http_parse_loads(char *data, size_t size)
{
    struct parse_prop_internal_ parse_prop_internal_ = {
        data, 0, size
    };

    return parse_prop_internal_;
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
        printf("starting %d end %d: %s\n", explode_map.explode_mapping_offset[i].starting_from, explode_map.explode_mapping_offset[i].ending_at, string);

        free(string);
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
                explode_map->explode_mapping_offset[explode_map->counting_arr - 1].ending_at = (i + 1);

                // change state
                first_literations = 0;
            } else {
                explode_map->explode_mapping_offset[explode_map->counting_arr - 1].starting_from = 
                    (explode_map->explode_mapping_offset[explode_map->counting_arr - 1 - 1].ending_at + 1);
                    
                explode_map->explode_mapping_offset[explode_map->counting_arr - 1].ending_at = (i + 1);
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