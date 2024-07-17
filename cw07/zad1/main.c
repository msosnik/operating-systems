#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define M 3  // Number of hairdressers
#define N 5  // Number of seats
#define P 10 // Number of waiting places
#define X 5  // Duration of a haircut in seconds

int waiting_count = 0;
int served_count = 0;
int current_client = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t hairdresser_cond = PTHREAD_COND_INITIALIZER;

void* hairdresser(void* arg) {
    int hairdresser_id = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (waiting_count == 0) {
            printf("Hairdresser %d is sleeping.\n", hairdresser_id);
            pthread_cond_wait(&client_cond, &mutex);
        }

        // Start serving the next client
        waiting_count--;
        printf("Hairdresser %d is cutting hair of client %d.\n", hairdresser_id, current_client++);
        served_count++;

        pthread_mutex_unlock(&mutex);
        sleep(X);

        pthread_cond_signal(&hairdresser_cond);
    }

    pthread_exit(NULL);
}

void* client(void* arg) {
    int client_id = *(int*)arg;

    pthread_mutex_lock(&mutex);

    if (waiting_count < P) {
        // Check if there are available seats
        if (waiting_count == N) {
            printf("Client %d is leaving because there are no free seats.\n", client_id);
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        waiting_count++;
        printf("Client %d is waiting.\n", client_id);
        pthread_cond_signal(&client_cond);
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        while (served_count < client_id) {
            pthread_cond_wait(&hairdresser_cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);

        printf("Client %d has finished the haircut.\n", client_id);
    } else {
        printf("Client %d is leaving because there are no free waiting places.\n", client_id);
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hairdressers[M];
    pthread_t clients[P + 1]; // +1 for the main client thread

    int hairdresser_ids[M];
    int client_ids[P + 1];

    // Create hairdresser threads
    for (int i = 0; i < M; i++) {
        hairdresser_ids[i] = i + 1;
        pthread_create(&hairdressers[i], NULL, hairdresser, &hairdresser_ids[i]);
    }

    // Create client threads
    for (int i = 0; i < P + 1; i++) {
        client_ids[i] = i + 1;
        pthread_create(&clients[i], NULL, client, &client_ids[i]);
        sleep(1); // Delay between creating clients
    }

    // Wait for hairdresser threads to finish
    for (int i = 0; i < M; i++) {
        pthread_join(hairdressers[i], NULL);
    }

    // Wait for client threads to finish
    for (int i = 0; i < P + 1; i++) {
        pthread_join(clients[i], NULL);
    }

    return 0;
}
