#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct account {
  uint32_t id;
  uint64_t balance;
  pthread_mutex_t lock;
};

struct withdraw_args {
  struct account* account;
  uint64_t amount;
};

void* withdraw(void* arg) {
  struct withdraw_args* args = (struct withdraw_args*)arg;
  struct account* account = args->account;
  uint64_t amount = args->amount;

  if (amount > 3000) {
    amount += 450;
  } else {
    amount += 300;
  }

  pthread_mutex_lock(&(account->lock));
  if (account->balance >= amount) {
    account->balance -= amount;
  } else {
    printf("Transaction faild: balance is not enough\n");
  }
  pthread_mutex_unlock(&(account->lock));
  return NULL;
}

int main() {
  pthread_t threads[4];
  struct account account_a = {
      .id = 1, .balance = 12000, PTHREAD_MUTEX_INITIALIZER};
  struct account account_b = {
      .id = 2, .balance = 4000, PTHREAD_MUTEX_INITIALIZER};

  struct withdraw_args args_a1 = {.account = &account_a, .amount = 4000};
  struct withdraw_args args_a2 = {.account = &account_a, .amount = 5000};
  struct withdraw_args args_b1 = {.account = &account_b, .amount = 2000};
  struct withdraw_args args_b2 = {.account = &account_b, .amount = 5000};

  pthread_create(&threads[0], NULL, withdraw, &args_a1);
  pthread_create(&threads[1], NULL, withdraw, &args_a2);
  pthread_create(&threads[2], NULL, withdraw, &args_b1);
  pthread_create(&threads[3], NULL, withdraw, &args_b2);

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);
  pthread_join(threads[2], NULL);
  pthread_join(threads[3], NULL);

  printf("account a: balance %llu\n", account_a.balance);
  printf("account b: balance %llu\n", account_b.balance);
  return 0;
}
