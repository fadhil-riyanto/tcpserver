#include <stdio.h>

int main()
{
    int a = ((((0x11 & 0x10) | (0x10 | 0x00)) == 0x11) ? 1 : 0); 
    printf("%d\n", a);
}