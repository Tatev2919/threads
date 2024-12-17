#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> 
#include <string.h>

void* thread_function(void* arg) {
    char* message = (char*)arg;
    printf("%s\n", message);  
    free(message);           
    return NULL;
}

int main() {
    pthread_t threads[3];

    for (int i = 0; i < 3; ++i) {
        char* message_ = (char*)malloc(50 * sizeof(char)); 
        if (message_ == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }

        sprintf(message_, "Hello from Thread %d", i + 1);

        if (pthread_create(&threads[i], NULL, thread_function, (void*)message_) != 0) {
            perror("Failed to create thread");
            free(message_);
            return 1;
        }
    }

    for (int i = 0; i < 3; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            return 1;
        }
    }

    printf("All threads have completed.\n");
    return 0;
}

