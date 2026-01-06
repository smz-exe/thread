#include <stdio.h>
#include <pthread.h>

void *thread(void *arg)
{
    for (;;)
    {
        char *s = arg;
        printf("Thread (%s)\n", s);
    }
    return NULL;
}

int main()
{
    pthread_t a, b;
    char arg_a[] = "I am a boy";
    char arg_b[] = "I am a girl";

    pthread_create(&a, NULL, thread, arg_a);
    pthread_create(&b, NULL, thread, arg_b);

    pthread_join(a, NULL);
    pthread_join(b, NULL);

    return 0;
}