#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <time.h> 

#define MAX_THREADS 4 // Defines the maximum number of threads to limit memory usage.
pthread_mutex_t mutex; // Mutex for synchronizing access to shared resources.

// Global counter for the number of prime numbers found. This is shared across threads.
int total_counter = 0;

// Function to check if a number is prime. This uses a simple algorithm checking divisibility
// of numbers less than or equal to the square root of the number being checked.
bool isPrime(int num) {
    if (num <= 1) return false; // 1 and less are not prime numbers.
    if (num == 2) return true; // 2 is a prime number.
    if (num % 2 == 0) return false; // Any even number other than 2 is not prime.
    for (int i = 3; i <= sqrt(num); i += 2) { // Check only odd numbers starting from 3.
        if (num % i == 0) return false; // If divisible, num is not prime.
    }
    return true; // If no divisors were found, num is prime.
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

// The main function initializes the mutex, creates threads, and starts each one
// by executing threadFunction. Finally, it waits for all threads to complete and
// then cleans up by destroying the mutex.
int main() {
    pthread_t threads[MAX_THREADS]; // Array to hold thread identifiers.
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex.

    // Create threads
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, threadFunction, NULL) != 0) {
            perror("Failed to create thread"); // Error handling if thread creation fails.
            return 1; // Return with error code.
        }
    }

    // Wait for all threads to complete their execution.
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL); // Join each thread.
    }

    // Print the total number of primes found by all threads.
    printf("%d total primes.\n", total_counter);
    pthread_mutex_destroy(&mutex); // Clean up by destroying the mutex.
    return 0; // Return 0 indicating successful execution.
}
