#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

struct account
{
    uint32_t id;
    uint64_t balance;
    pthread_mutex_t lock;
};

struct withdraw_args
{
    struct account *account;
    uint64_t amount;
};

void *withdraw(void *arg)
{
    struct withdraw_args *args = (struct withdraw_args *)arg;
    struct account *account = args->account;
    uint64_t amount = args->amount;

    bool can_withdraw;

    if (amount < 3000)
    {
        amount += 220;
    }
    else
    {
        amount += 450;
    }

    pthread_mutex_lock(&(account->lock));
    if (account->balance >= amount)
    {
        account->balance -= amount;
    }
    else
    {
        printf("Transaction failed: insufficient funds\n");
    }
    pthread_mutex_unlock(&(account->lock));

    return NULL;
}

struct transfer_args
{
    struct account *sender;
    struct account *receiver;
    uint64_t amount;
};

void *transfer(void *arg)
{
    struct transfer_args *args = (struct transfer_args *)arg;
    struct account *sender = args->sender;
    struct account *receiver = args->receiver;
    uint64_t amount = args->amount;

    struct account *first = sender;
    struct account *second = receiver;

    if (first->id > second->id)
    {
        first = receiver;
        second = sender;
    }

    pthread_mutex_lock(&(first->lock));
    pthread_mutex_lock(&(second->lock));

    if (sender->balance >= amount)
    {
        sender->balance -= amount;
        receiver->balance += amount;
    }
    else
    {
        printf("Transaction failed: insufficient funds\n");
    }

    pthread_mutex_unlock(&(second->lock));
    pthread_mutex_unlock(&(first->lock));
    return NULL;
}

int main()
{
    pthread_t w1, w2, t;

    struct account account_a = {.id = 1, .balance = 10000, .lock = PTHREAD_MUTEX_INITIALIZER};
    struct account account_b = {.id = 2, .balance = 7500, .lock = PTHREAD_MUTEX_INITIALIZER};

    struct withdraw_args w_args_a = {.account = &account_a, .amount = 3500};
    struct withdraw_args w_args_b = {.account = &account_b, .amount = 1000};

    struct transfer_args t_args = {.sender = &account_a, .receiver = &account_b, .amount = 4000};

    pthread_create(&w1, NULL, withdraw, &w_args_a);
    pthread_create(&w2, NULL, withdraw, &w_args_b);
    pthread_create(&t, NULL, transfer, &t_args);

    pthread_join(w1, NULL);
    pthread_join(w2, NULL);
    pthread_join(t, NULL);

    printf("Account A: balance=%llu \n", account_a.balance);
    printf("Account B: balance=%llu \n", account_b.balance);
}