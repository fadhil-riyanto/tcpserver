#include "header/uriparser.h"
#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header/utils.h"

void internal_urlparser(struct internal_dataparser *internal_dataparser)
{
    static char pathstring[200];
    char paramsstring[200];
    char paramsraw[200];
    char key[200];
    char value[200];
    int ret, ret2;
    struct datastore_session *ret_realloc;

    ret = split('?', internal_dataparser->source, internal_dataparser->length, 0, pathstring);
    if (ret != 2) {
        internal_dataparser->path = pathstring;
    } else {
        internal_dataparser->path = 0;
    }
    
    ret = split('?', internal_dataparser->source, internal_dataparser->length, 1, paramsstring);
    if (ret != 3) // index overflow here
    {
        int i = 0;
        do {
            ret = split('&', paramsstring, strlen(paramsstring), i, paramsraw);
            
                
            if (ret == 0 || ret == 1) {
                // printf("gate here executed\n");
                // realloc here
                internal_dataparser->capacity = internal_dataparser->capacity + 1;
                internal_dataparser->datastore_session = realloc(internal_dataparser->datastore_session, sizeof(struct datastore_session) * internal_dataparser->capacity);
                
                // if (!ret_realloc) {
                //     internal_dataparser->datastore_session = ret_realloc;
                // }
                // printf("inrdex ret %s num %d: %s\n", ((ret == 2) ? "true" : "false"), ret, paramsraw);
            
                internal_dataparser->datastore_session[i].indexof = i;
                
                ret2 = split('=', paramsraw, strlen(paramsraw), 0, key);
                strcpy(internal_dataparser->datastore_session[i].key, key);

                ret2 = split('=', paramsraw, strlen(paramsraw), 1, value);
                strcpy(internal_dataparser->datastore_session[i].value, value);
                // printf("%s\n", key);
            } 
            // 2nd parse stack
            // ret2 = split('=', paramsstring, strlen(paramsstring), i, key);


            // }
            i++;
        } while (!(ret == 2 || ret == 1));    
    }

    // split text
    // for(int i = 0; i < 3; i++) {
    
    

    // printf("%s\n", pathstring);
}

void urlparser_parse(struct internal_dataparser *internal_dataparser, char *text, int length)
{
    internal_dataparser->source = text;
    internal_dataparser->length = length;
    internal_dataparser->capacity = 1;
    internal_dataparser->datastore_session = malloc(sizeof(struct datastore_session) * internal_dataparser->capacity); // initial value is 1

    internal_urlparser(internal_dataparser);
}

char* uriparser_get_path(struct internal_dataparser *internal_dataparser)
{
    return internal_dataparser->path;
}

void urlparser_cleanup(struct internal_dataparser *internal_dataparser)
{
    free(internal_dataparser->datastore_session);
}

char* uriparser_get_params(struct internal_dataparser *internal_dataparser, char* key)
{
    // char tempchar[200];
    for(int i = 0; i < internal_dataparser->capacity; i++) {
        if (strcmp(key, internal_dataparser->datastore_session[i].key) == 0) {
            // printf("%s\n", internal_dataparser->datastore_session[i].value);
            // strcpy(tempchar, internal_dataparser->datastore_session[i].value);
            return internal_dataparser->datastore_session[i].value;
        } 
    }
    return 0;

    // printf("%s\n", internal_dataparser->datastore_session[1].key);
}

char* urlparser_stringfy(struct internal_dataparser *internal_dataparser)
{
    char temp[(200 * 2) + 9];
    char* ret = (char*)malloc((((sizeof(char) * 200) * 2) + 9) * internal_dataparser->capacity);
    for(int i = 0; i < internal_dataparser->capacity - 1; i++) {
        snprintf(temp, sizeof(temp), "\"%s\" => \"%s\",", internal_dataparser->datastore_session[i].key, internal_dataparser->datastore_session[i].value);
        
        strcat(ret, temp);
    }
    // printf("%s\n", ret);
    return ret;
}