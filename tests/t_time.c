#include <time.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    for(;;)
    {
        sleep(1);
        printf("%d\n", (int)time(NULL));
    }
    
}