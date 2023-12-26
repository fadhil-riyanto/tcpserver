
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *targetf(void *data)
{
    sleep(1);
    printf("hi");
}

int main()
{
    pthread_t threadid;

    pthread_create(&threadid, NULL, targetf, NULL);
    
    
    // expect: all freed
    //pthread_join(threadid, 0); // <- freed

    //pthread_detach(threadid);
    sleep(2);
    return 0;
}