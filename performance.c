#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <time.h> 
#include "queue.h"

#define MAX_THREADS 4 
pthread_mutex_t mutex; 


int total_counter = 0;
myQueue q;

// Function to check if a number is prime. This uses a simple algorithm checking divisibility
// of numbers less than or equal to the square root of the number being checked.
bool isPrime(int num) {
    if (num <= 1) return false; 
    if (num == 2) return true; 
    if (num % 2 == 0) return false; 
    for (int i = 3; i <= sqrt(num); i += 2) { 
        if (num % i == 0) return false; 
    }
    return true; 
}

// This is the function that each thread will execute. It reads numbers from stdin,
// checks if they are prime, and updates the global count of prime numbers found.
void* threadFunction(void* arg) {
    int num;
    while (true) {
        pthread_mutex_lock(&mutex); // Lock the mutex before reading stdin to avoid race conditions.
        if (scanf("%d", &num) == EOF) { // Read a number and check for the end of input.
            pthread_mutex_unlock(&mutex); // Unlock the mutex if the end of input is reached.
            break; // Exit the loop if no more numbers are available.
        }
        pthread_mutex_unlock(&mutex); // Unlock the mutex after reading a number.

        if (isPrime(num)) {
            pthread_mutex_lock(&mutex); // Lock the mutex before updating the global counter.
            total_counter++; // Increment the global counter if the number is prime.
            pthread_mutex_unlock(&mutex); // Unlock the mutex after updating.
        }
    }
    return NULL; // Return NULL as this thread function does not return anything.
}


int main() {
    int num;
    pthread_t *threads = malloc(MAX_THREADS * sizeof(pthread_t));
    if (threads == NULL)
	{
		perror("error");
		exit(1);
	}
    int *threadD = malloc(MAX_THREADS * sizeof(int));
    if (threadD == NULL)
	{
		perror("error");
		exit(1);
	}

    createQueue(&q);

    // Create threads
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, threadFunction, NULL) != 0) {
            perror("Failed to create thread"); // Error handling if thread creation fails.
            return 1; 
        }
    }

    while (scanf("%d", &num) != EOF) {   
        if (isPrime(num)) {
            total_counter++;
        }
    }


    // Wait for all threads to complete their execution.
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL); // Join each thread.
    }

    // Print the total number of primes found by all threads.
    printf("%d total primes.\n", total_counter);

    free(threads);

    return 0; // Return 0 indicating successful execution.
}