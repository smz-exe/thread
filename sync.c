#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

struct account
{
    u_int32_t id;
    u_int64_t balance;
    pthread_mutex_t lock;
};

struct withdraw_args
{
    struct account *account;
    u_int64_t amount;
};

void *withdraw(void *arg)
{
    struct withdraw_args *args = (struct withdraw_args *)arg;
    struct account *account = args->account;
    u_int64_t amount = args->amount;

    bool can_withdraw;

    if (amount < 3000)
    {
        amount = amount + 220;
    }
    else
    {
        amount = amount + 320;
    }

    pthread_mutex_lock(&account->lock);
    can_withdraw = account->balance >= amount;
    if (can_withdraw)
    {
        account->balance = account->balance - amount;
    }
    pthread_mutex_unlock(&account->lock);

    return NULL;
}

int main()
{
    pthread_t a, b;
    struct account account_a = {.id = 1, .balance = 10000, .lock = PTHREAD_MUTEX_INITIALIZER};
    struct account account_b = {.id = 2, .balance = 8000, .lock = PTHREAD_MUTEX_INITIALIZER};

    int amount_a = 4500;
    int amount_b = 2000;

    struct withdraw_args args_a = {.account = &account_a, .amount = amount_a};
    struct withdraw_args args_b = {.account = &account_b, .amount = amount_b};

    pthread_create(&a, NULL, withdraw, &args_a);
    pthread_create(&b, NULL, withdraw, &args_b);

    pthread_join(a, NULL);
    pthread_join(b, NULL);

    int expected_a = 10000 - (amount_a + 320);
    int expected_b = 8000 - (amount_b + 220);
    printf("Account A: balance=%llu, expected=%d\n", account_a.balance, expected_a);
    printf("Account B: balance=%llu, expected=%d\n", account_b.balance, expected_b);

    return 0;
}