#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

sem_t *semaphore;

void *task(void *arg)
{
    int id = *(int *)arg;

    printf("Thread %d: Waiting...\n", id);
    sem_wait(semaphore);

    printf("Thread %d: Entered critical section\n", id);
    sleep(2);
    printf("Thread %d: Leaving critical section\n", id);

    sem_post(semaphore);
    return NULL;
}

int main()
{
    pthread_t threads[5];
    int ids[5];

    sem_unlink("/mysem"); // Remove if it exists from a previous run
    semaphore = sem_open("/mysem", O_CREAT | O_EXCL, 0644, 1);
    if (semaphore == SEM_FAILED)
    {
        perror("sem_open");
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, task, &ids[i]);
    }

    for (int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_close(semaphore);
    sem_unlink("/mysem");
    return 0;
}