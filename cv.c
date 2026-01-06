#include <pthread.h>
#define N 5

int buffer[N];
int inptr = 0, outptr = 0;
int count = 0;

pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void producer()
{
    int data;
    for (;;)
    {
        data = produce();
        pthread_mutex_lock(&lock);
        while (count >= N)
            pthread_cond_wait(&full, &lock);
        buffer[inptr] = data;
        inptr = (inptr + 1) % N;
        count = count + 1;
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&lock);
    }
}

void consumer()
{
    int data;
    for (;;)
    {
        pthread_mutex_lock(&lock);
        while (count == 0)
            pthread_cond_wait(&empty, &lock);
        buffer[outptr];
        outptr = (outptr + 1) % N;
        count = count - 1;
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&lock);
        consume(data);
    }
}