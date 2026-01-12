#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/semaphore.h>
#include <unistd.h>

#define PARKING_SPACES 5
#define NUM_CARS 10

sem_t* parking_lot;

void* car(void* arg) {
  int id = *(int*)arg;

  usleep((rand() % 500) * 1000);
  printf("Car %d: Arrived, looking for parking...\n", id);
  sem_wait(parking_lot);

  printf("Car %d: PARKED (staying for a while)\n", id);
  sleep(2 + rand() % 2);
  printf("Car %d: Leaving parking lot\n", id);

  sem_post(parking_lot);
  return NULL;
}

int main() {
  pthread_t threads[NUM_CARS];
  int ids[NUM_CARS];

  srand(42);

  printf("Parking spaces: %d, Cars: %d\n", PARKING_SPACES, NUM_CARS);

  sem_unlink("/parking");
  parking_lot = sem_open("/parking", O_CREAT | O_EXCL, 0644, PARKING_SPACES);

  if (parking_lot == SEM_FAILED) {
    perror("sem_open");
    return 1;
  }

  for (int i = 0; i < NUM_CARS; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, car, &ids[i]);
  }

  for (int i = 0; i < NUM_CARS; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_close(parking_lot);
  sem_unlink("/parking");
  return 0;
}
