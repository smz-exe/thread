#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *worker(void *arg)
{
    for (int i = 0; i < 5; i++)
    {
        printf("[worker] still running (i=%d)\n", i);
        sleep(1);
    }
    return NULL;
}

void start_worker_then_return(void)
{
    pthread_t t;
    pthread_create(&t, NULL, worker, NULL);

    pthread_detach(t);
    printf("[start_worker_then_return] returning now; t goes out of scope.\n");
}

int main(void)
{
    start_worker_then_return();

    for (int i = 0; i < 8; i++)
    {
        printf("[main] alive (i=%d)\n", i);
        sleep(1);
    }
    return 0;
}