#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <time.h> 

#define MAX_THREADS 4 // Defines the maximum number of threads to limit memory usage.

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

// Thread function to count prime numbers within a range of the array.
void* countPrimes(void* arg) {
    PrimeWork* work = (PrimeWork*)arg; // Cast argument to PrimeWork pointer.
    for (int i = work->start; i < work->end; i++) { // Process assigned range.
        if (isPrime(work->numbers[i])) {
            pthread_mutex_lock(&lock); // Lock to safely update the global counter.
            total_counter++;
            pthread_mutex_unlock(&lock); // Unlock after updating.
        }
    }
    return NULL; // Thread completed its work.
}

int main() {
    int num, numOfNumbers = 0;

    int capacity = 10; // Initial capacity of the numbers array.
    int* numbers = (int*)malloc(capacity * sizeof(int)); // Dynamically allocate array.

    int maxIntegers = 2097152 / sizeof(int); // Maximum number of integers within 2MB.

    // Read all numbers from stdin into the array.
    while (scanf("%d", &num) != EOF) {
        if (numOfNumbers >= capacity) { // Array is full, need more space.
            if (capacity * 2 > maxIntegers) {
                printf("Maximum capacity reached. Cannot allocate more than 2MB.\n");
                break; // Stop reading more numbers to avoid exceeding 2MB.
            }
            capacity = (capacity * 2 <= maxIntegers) ? capacity * 2 : maxIntegers; // Ensure not to exceed maxIntegers.
            numbers = (int*)realloc(numbers, capacity * sizeof(int)); // Reallocate with new capacity.
        }
        if (numOfNumbers < maxIntegers) {
            numbers[numOfNumbers++] = num; // Store the number and increment count.
        } else {
            printf("Maximum capacity reached. Cannot store more numbers.\n");
            break; // Stop reading more numbers to avoid exceeding 2MB.
        }
    }

    // Initialize mutex for thread safety.
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("erro");
        return 1;
    }

    pthread_t tid[MAX_THREADS]; // Array to hold thread IDs.
    PrimeWork works[MAX_THREADS]; // Array of work structs for each thread.
    int chunkSize = numOfNumbers / MAX_THREADS; // Calculate workload per thread.

    // Create threads and assign work.
    for (int i = 0; i < MAX_THREADS; i++)
    {
        works[i].numbers = numbers;     // Point all works to the same numbers array.
        works[i].start = i * chunkSize; // Calculate start index.
        // Calculate end index, handling any remaining numbers in the last thread.
        if (i == MAX_THREADS - 1)
        {
            works[i].end = numOfNumbers; // Last thread takes any remaining numbers
        }
        else
        {
            works[i].end = (i + 1) * chunkSize; // Other threads take a defined chunk
        }

        // Create thread to execute countPrimes function with works[i] as argument.
        if (pthread_create(&tid[i], NULL, countPrimes, (void*)&works[i]) != 0) {
            perror("Failed to create thread");
            free(numbers); // Cleanup allocated memory on failure.
            return 1;
        }
    }

    // Wait for all threads to finish.
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    // Output the total count of prime numbers found.
    printf("%d total primes.\n", total_counter);

    // Cleanup and exit.
    pthread_mutex_destroy(&lock); // Destroy the mutex.
    free(numbers); // Free the dynamically allocated numbers array.

    return 0;
}
