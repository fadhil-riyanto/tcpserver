#include <stdio.h>
#include <stdlib.h>

int main()
{
    int val = 0;

    to:
    if (val != 0)
    {
        printf("value isnt null\n");
        exit(1);
    }


    printf("here go\n");
    val = 1;
    goto to;


}