#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

const int n = 10;
bool choosing[n] = {false};
int number[n] = {0};

int shared_variable = 5;
pthread_mutex_t lock;

void random_delay() {
    int delay = rand() % 1000;
    usleep(delay);
}

void* thread_function(void* arg) {
    int me = *((int*)arg);
    free(arg);

    choosing[me] = true;
    number[me] = *std::max_element(number, number + n) + 1;
    choosing[me] = false;

    for (int j = 0; j < n; ++j) {
        while (choosing[j]) { random_delay(); }

        while ((number[j] != 0) && ((number[j] < number[me]) || (number[j] == number[me] && j < me))) {
            random_delay();
        }
    }

    pthread_mutex_lock(&lock);

    printf("Thread %d is entering the critical section\n", me + 1);

    int local_copy = shared_variable;

    random_delay();
    local_copy++;
    random_delay();
    local_copy--;
    random_delay();

    shared_variable = local_copy;

    printf("Thread %d updated shared_variable to: %d\n", me + 1, shared_variable);

    pthread_mutex_unlock(&lock);

    number[me] = 0;

    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t threads[n];

    if (pthread_mutex_init(&lock, NULL) != 0) {
        perror("Failed to initialize mutex");
        return 1;
    }

    for (int i = 0; i < n; ++i) {
        int* thread_id = (int*)malloc(sizeof(int));
        if (thread_id == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }
        *thread_id = i;

        if (pthread_create(&threads[i], NULL, thread_function, (void*)thread_id) != 0) {
            perror("Failed to create thread");
            free(thread_id);
            return 1;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    pthread_mutex_destroy(&lock);

    printf("All threads have completed. Final shared_variable value: %d\n", shared_variable);
    return 0;
}
