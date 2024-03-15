#include "uriparser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 
int main()
{
    struct internal_dataparser internal_dataparser;    
    internal_dataparser.path = (char*)malloc(sizeof(char) * 200);
    char *text = "/test?t=ffab&q=cara+tests&atb=v385-1&data=fadhil&source=firefox";
    // char *text = "/testss"; // none
    urlparser_parse(&internal_dataparser, text, strlen(text));

    printf("path: %s\n", uriparser_get_path(&internal_dataparser));
    // printf("param: %s\n", uriparser_get_params(&internal_dataparser, "atb"));
    printf("%s\n", urlparser_stringfy(&internal_dataparser));


    urlparser_cleanup(&internal_dataparser);
}