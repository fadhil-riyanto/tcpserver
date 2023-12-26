#include <stdio.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>

void* runfunc(void* arguments){
    printf("in run thread\n");
    return NULL;
}

int main(void)
{
    pthread_t pthread_id;

    pthread_create(&pthread_id, NULL, runfunc, NULL);

    printf("after pthread creation\n");
    sleep(1);

    return 0;
}