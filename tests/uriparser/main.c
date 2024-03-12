#include "uriparser.h"
#include <string.h>
#include <stdio.h>
 
int main()
{
    struct internal_dataparser internal_dataparser;    
    // char *text = "/test?t=ffab&q=cara+tests&atb=v385-1";
    char *text = ""; // none
    urlparser_parse(&internal_dataparser, text, strlen(text));

    printf("path: %s\n", uriparser_get_path(&internal_dataparser));
    printf("param: %s\n", uriparser_get_params(&internal_dataparser, "atb"));

    urlparser_cleanup(&internal_dataparser);
}