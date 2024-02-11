#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <time.h> 

#define MAX_THREADS 4
#define MAX_PRIME_LIMIT 100000 // Adjust based on your expected input range
#define MAX_PRIMES 9592 // Number of primes under 100,000. Adjust according to MAX_PRIME_LIMIT

pthread_mutex_t mutex;
int total_counter = 0;
int small_primes[MAX_PRIMES];
int num_small_primes = 0;

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

bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 0; i < num_small_primes; i++) {
        if (num == small_primes[i]) return true;
        if (num % small_primes[i] == 0) return false;
    }
    for (int i = small_primes[num_small_primes-1]+1; i <= sqrt(num); i++) {
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
    pthread_t threads[MAX_THREADS];
    pthread_mutex_init(&mutex, NULL);

    // Generate small primes up to a specified limit before starting the threads
    generateSmallPrimes(MAX_PRIME_LIMIT);

    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, threadFunction, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", total_counter);
    pthread_mutex_destroy(&mutex);
    return 0;
}
