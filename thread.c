#include <stdio.h>
#include <pthread.h>

static pthread_t a, b;

void *threadA(void *arg)
{
    for (;;)
        printf("Thread A\n");
    return NULL;
}

void *threadB(void *arg)
{
    for (;;)
        printf("Thread B\n");
    return NULL;
}

int main()
{
    pthread_create(&a, NULL, threadA, NULL);
    pthread_create(&b, NULL, threadB, NULL);

    pthread_join(a, NULL);
    pthread_join(b, NULL);

    return 0;
}
