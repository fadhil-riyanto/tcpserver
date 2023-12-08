#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int ret = 0;
    int tempret = 0;
    int fd = open("txt", O_RDONLY);
    char buf[100];

    for(int i = 0; i <= 13; i++)
    {
        
        ret = read(fd, &buf[9], 3);
        tempret = ret;
    }
    

    printf("readed %s\n", buf);
    printf("ret return %d", ret);
}