#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <time.h> 

#define MAX_THREADS 4 // Defines the maximum number of threads to use
#define MAX_PRIME_LIMIT 100000 // Maximum number up to which to precompute primes
#define MAX_PRIMES 9592 // Number of primes under 100,000 (precomputed)

pthread_mutex_t mutex; // Mutex for synchronizing access to shared resources
int total_counter = 0; // Global counter for the total number of primes found
int small_primes[MAX_PRIMES]; // Array to store precomputed small primes
int num_small_primes = 0; // Counter for the number of precomputed small primes

// Function to generate small primes up to a specified limit using the Sieve of Eratosthenes
void generateSmallPrimes(int limit) {
    bool prime[limit+1];
    for (int i = 0; i <= limit; i++) prime[i] = true;
    for (int p = 2; p*p <= limit; p++) {
        if (prime[p] == true) {
            for (int i = p*p; i <= limit; i += p)
                prime[i] = false;
        }
    }
    for (int p = 2; p <= limit; p++) {
        if (prime[p]) {
            small_primes[num_small_primes++] = p;
        }
    }
}

// Function to check if a number is prime by first comparing against precomputed primes,
// then by trial division for numbers larger than precomputed primes
bool isPrime(int num) {
    if (num <= 1) return false; // Numbers less than 2 are not prime
    for (int i = 0; i < num_small_primes; i++) {
        if (num == small_primes[i]) return true; // Number is a precomputed prime
        if (num % small_primes[i] == 0) return false; // Number is divisible by a precomputed prime
    }
    for (int i = small_primes[num_small_primes-1]+1; i <= sqrt(num); i++) {
        if (num % i == 0) return false; // Number is divisible by a number larger than the largest precomputed prime
    }
    return true; // Number passed all checks; it is prime
}

#define MAX_BATCH_SIZE 10 // Defines the maximum number of numbers to read at once

// Thread function to read numbers in batches, check for primality, and update the total prime count
void* threadFunction(void* arg) {
    int local_counter = 0; // Counter for primes found by this thread
    int numbers[MAX_BATCH_SIZE]; // Buffer for batch processing numbers

    while (true) {
        int batch_size = 0; // Size of the current batch

        // Critical section to read a batch of numbers from stdin
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < MAX_BATCH_SIZE; i++) {
            if (scanf("%d", &numbers[i]) == EOF) break; // Break if EOF is reached
            batch_size++; // Increment batch size for each number read
        }
        pthread_mutex_unlock(&mutex);

        if (batch_size == 0) break; // Exit the loop if no numbers were read

        // Process each number in the batch without holding the mutex
        for (int i = 0; i < batch_size; i++) {
            if (isPrime(numbers[i])) {
                local_counter++; // Increment local counter if number is prime
            }
        }
    }

    // Critical section to update the global counter with the count from this thread
    pthread_mutex_lock(&mutex);
    total_counter += local_counter;
    pthread_mutex_unlock(&mutex);

    return NULL; // Return NULL as required by pthread_create()
}

int main() {
    pthread_t threads[MAX_THREADS]; // Array to hold thread identifiers
    pthread_mutex_init(&mutex, NULL); // Initialize the mutex

    // Precompute small primes before starting the threads
    generateSmallPrimes(MAX_PRIME_LIMIT);

    // Create threads to process input
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, threadFunction, NULL) != 0) {
            perror("Failed to create thread"); // Error handling if thread creation fails
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the total number of primes found
    printf("%d total primes.\n", total_counter);
    pthread_mutex_destroy(&mutex); // Clean up the mutex
    return 0;
}
