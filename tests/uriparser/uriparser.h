#ifndef URIPARSER_H_
#define URIPARSER_H_

struct datastore_session
{
    int indexof;
    char key[200];
    char value[200];
};

struct internal_dataparser
{
    char *source;
    int length;
    
    struct datastore_session *datastore_session;
    int capacity;

    char *path;
};

void urlparser_parse(struct internal_dataparser *internal_dataparser, char *text, int length);
char* uriparser_get_params(struct internal_dataparser *internal_dataparser, char* key);
char* uriparser_get_path(struct internal_dataparser *internal_dataparser); // return path of request
void urlparser_cleanup(struct internal_dataparser *internal_dataparser);
char* urlparser_stringfy(struct internal_dataparser *internal_dataparser);


#endif