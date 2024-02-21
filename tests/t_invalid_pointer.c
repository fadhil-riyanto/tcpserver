#include <stdio.h>

int dummy(char **data)
{
    // nothing
}

int main()
{
    char *data;
    dummy(&data);

    printf("%s\n", data);
}