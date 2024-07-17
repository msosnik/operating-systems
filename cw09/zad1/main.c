#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_REINDEER 9
#define NUM_ELVES 10
#define MAX_ELVES_WAITING 3

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeer_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t elves_cond = PTHREAD_COND_INITIALIZER;

int reindeer_arrived = 0;
int elves_waiting = 0;
int gifts_delivered = 0;

void* reindeer_thread(void* arg) {
    int id = *(int*)arg;

    while (gifts_delivered < 3) {
        int holiday_duration = 5 + rand() % 6;

        sleep(holiday_duration);
        pthread_mutex_lock(&santa_mutex);
        reindeer_arrived++;

        printf("Reindeer: %d are waiting for Santa, id %d\n", reindeer_arrived, id);

        if (reindeer_arrived == NUM_REINDEER) {
            printf("Reindeer: All reindeer have returned. %d is waking up Santa!\n", id);
            pthread_cond_signal(&reindeer_cond);
        }
        pthread_mutex_unlock(&santa_mutex);

        pthread_mutex_lock(&santa_mutex);
        pthread_cond_wait(&reindeer_cond, &santa_mutex);

        printf("Reindeer: %d has finished delivering gifts.\n", id);
        pthread_mutex_unlock(&santa_mutex);
    }

    pthread_exit(NULL);
}

void* elf_thread(void* arg) {
    int id = *(int*)arg;

    while (gifts_delivered < 3) {
        int work_duration = 2 + rand() % 4;

        sleep(work_duration);

        pthread_mutex_lock(&santa_mutex);
        elves_waiting++;
        printf("Elf: %d elves finished working, id %d\n", elves_waiting, id);

        if (elves_waiting == MAX_ELVES_WAITING) {
            printf("Elf: Waking up Santa! id %d\n", id);
            pthread_cond_signal(&elves_cond);
        } else if (elves_waiting > MAX_ELVES_WAITING) {
            printf("Elf: solves his problem alone, id %d\n", id);
            elves_waiting--;
        }
        pthread_mutex_unlock(&santa_mutex);

        pthread_mutex_lock(&santa_mutex);
        pthread_cond_wait(&elves_cond, &santa_mutex);
        printf("Elf: Santa finished helping, id %d.\n", id);

        pthread_mutex_unlock(&santa_mutex);
    }

    pthread_exit(NULL);
}

void* santa_thread(void* arg) {
    while (gifts_delivered < 3) {
        pthread_mutex_lock(&santa_mutex);
        while (reindeer_arrived < NUM_REINDEER && elves_waiting < MAX_ELVES_WAITING) {
            printf("Santa: sleeping.\n");
            pthread_cond_wait(&reindeer_cond, &santa_mutex);
//            pthread_cond_wait(&elves_cond, &santa_mutex);
        }

        if (elves_waiting >=MAX_ELVES_WAITING) {

            printf("Santa: waking up to solve elves' problems.\n");

            int help_duration = 1 + rand() % 2;
            sleep(help_duration);

            printf("Santa has solved elves problems.\n");
            elves_waiting -= MAX_ELVES_WAITING;
            pthread_cond_broadcast(&elves_cond);
        } else if (reindeer_arrived == NUM_REINDEER) {
            printf("Santa: waking up to deliver gifts.\n");

            int gift_duration = 2 + rand() % 3;
            sleep(gift_duration);

            gifts_delivered++;
            printf("Santa: delivered gifts %d times.\n", gifts_delivered);

            reindeer_arrived = 0;
            pthread_cond_broadcast(&reindeer_cond);
        }
        pthread_mutex_unlock(&santa_mutex);
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    pthread_t reindeer_threads[NUM_REINDEER];
    pthread_t elf_threads[NUM_ELVES];
    pthread_t santa_thread_id;

    int reindeer_ids[NUM_REINDEER];
    int elf_ids[NUM_ELVES];

    for (int i = 0; i < NUM_REINDEER; i++) {
        reindeer_ids[i] = i + 1;
        pthread_create(&reindeer_threads[i], NULL, reindeer_thread, &reindeer_ids[i]);
    }

    for (int i = 0; i < NUM_ELVES; i++) {
        elf_ids[i] = i + 1;
        pthread_create(&elf_threads[i], NULL, elf_thread, &elf_ids[i]);
    }

    pthread_create(&santa_thread_id, NULL, santa_thread, NULL);

    for (int i = 0; i < NUM_REINDEER; i++) {
        pthread_join(reindeer_threads[i], NULL);
    }

    for (int i = 0; i < NUM_ELVES; i++) {
        pthread_join(elf_threads[i], NULL);
    }

    pthread_join(santa_thread_id, NULL);



    return 0;
}
