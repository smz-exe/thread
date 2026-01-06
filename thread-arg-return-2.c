#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread(void *arg)
{
    for (;;)
    {
        printf("Thread (%d)\n", *(int *)arg);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t t[2];
    int i;

    for (i = 0; i < 2; i++)
    {
        printf("%d\n", i);
        pthread_create(&t[i], NULL, thread, &i);
    }

    for (int j = 0; j < 2; j++)
    {
        printf("In the second loop: %d\n", j);
        pthread_join(t[j], NULL);
    }

    return 0;
}