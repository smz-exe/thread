#include <pthread.h>
#include <stdio.h>
#define N 5

int buffer[N];
int inptr = 0, outptr = 0;
int count = 0, num = 0, result = 0;
int done = 0;

pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int produce()
{
    num++;
    return num;
}

void consume(int data)
{
    result += data;
}

void *producer(void *arg)
{
    int data;
    for (int i = 0; i < 1000; i++)
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

    pthread_mutex_lock(&lock);
    done = 1;
    pthread_cond_signal(&empty);
    pthread_mutex_unlock(&lock);

    return NULL;
}

void *consumer(void *arg)
{
    int data;
    while (1)
    {
        pthread_mutex_lock(&lock);

        while (count == 0 && !done)
            pthread_cond_wait(&empty, &lock);

        if (count == 0 && done)
        {
            pthread_mutex_unlock(&lock);
            break;
        }

        data = buffer[outptr];
        outptr = (outptr + 1) % N;
        count = count - 1;
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&lock);
        consume(data);
    }
    return NULL;
}

int main()
{
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    printf("result: %d\n", result);
    return 0;
}