#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* calculate_square(void* arg) {
    int number = *(int*)arg;
    int square = number * number;
    printf("Square of %d is %d\n", number, square);
    free(arg);
    return NULL;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    pthread_t threads[size];

    for (int i = 0; i < size; ++i) {
        int* num = (int*)malloc(sizeof(int));
        if (num == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }

        *num = numbers[i];

        if (pthread_create(&threads[i], NULL, calculate_square, (void*)num) != 0) {
            perror("Failed to create thread");
            free(num);
            return 1;
        }
    }

    for (int i = 0; i < size; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    printf("All threads have completed.\n");
    return 0;
}

