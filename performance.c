#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <time.h> 

#define MAX_THREADS 4 // Defines the maximum number of threads to limit memory usage.
pthread_mutex_t mutex;

pthread_mutex_t lock; // Mutex for protecting the shared total_counter variable.
int total_counter = 0;  // Global counter for the number of prime numbers found.

// Structure to hold information for each thread about the range of numbers to process.
typedef struct {
    int* numbers; // Pointer to the array of numbers to check.
    int start;    // Start index in the array for this thread's work.
    int end;      // End index in the array for this thread's work (exclusive).
} PrimeWork;

// Function to check if a number is prime.
bool isPrime(int num) {
    if (num <= 1) return false; 
    if (num == 2) return true; 
    if (num % 2 == 0) return false; 
    for (int i = 3; i <= sqrt(num); i += 2) { 
        if (num % i == 0) return false; 
    }
    return true; 
}

void* threadFunction(void* arg) {
    int num;
    while (true) {
        pthread_mutex_lock(&mutex);
        if (scanf("%d", &num) == EOF) { // Check for end of input
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        if (isPrime(num)) {
            pthread_mutex_lock(&mutex);
            total_counter++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}


int main() {
    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, threadFunction, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Join threads
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", total_counter);
    pthread_mutex_destroy(&mutex);
    return 0;
}

