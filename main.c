// #include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <pthread.h>
// #include <math.h>
// #include <unistd.h>
// #include <sys/sysinfo.h>
// #include <sys/time.h>
// #include <semaphore.h>
// #define MAX_THREADS 4  // Limit number of threads to manage memory usage

// #define NUM_T 2
// #define NUM_T_L 1
// // pthread_t* tid;
// pthread_mutex_t lock;
// long sum = 0;
// int primeCounter = 0;  // Global counter for primes
// long numOfRandomNumbers;

// int randomPivot;
// long Thread_num;
// int y;
// int x;
// // Function to check if a number is prime
// bool isPrime(int num) {
//     if (num <= 1) return false;
//     if (num % 2 == 0 && num > 2) return false;
//     for (int i = 3; i <= sqrt(num); i += 2) {
//         if (num % i == 0) return false;
//     }
//     return true;
// }

// void* countPrimes(int num) {
//     while (scanf("%d", &num) != EOF) {
//         if (isPrime(num)) {
//             pthread_mutex_lock(&lock);
//             primeCounter++;
//             pthread_mutex_unlock(&lock);
//         }
//     }
//     return NULL;
// }



// int main()
// {

//     if (pthread_mutex_init(&lock, NULL) != 0)
//     {
//         printf("erro");
//         return 1;
//     }

//     int e;

//     // Thread_num = get_nprocs_conf();// Number of processors

//     pthread_t tid[MAX_THREADS];
//     int num;
//     // int total_counter = 0;

    
//     for (int i = 0; i < MAX_THREADS ; i++)
//     {

//         e = pthread_create(&(tid[i]), NULL, &countPrimes, num);
//         //check if the number is prime
//         if (e != 0)
//         {
//             perror("Failed to create thread");
//             return 1;
//         }
//     }
    
//     // Wait for all threads to finish
//     for (int i = 0; i < MAX_THREADS; i++)
//     {
//         pthread_join(tid[i], NULL);
//     }

//     //keep the out format as this!!
//     printf("%d total primes.\n", primeCounter);

//     // Clean up and exit
//     pthread_mutex_destroy(&lock);
    
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <time.h> 

#define MAX_THREADS 4

pthread_mutex_t lock;
int primeCounter = 0;  // Global counter for primes

typedef struct {
    int* numbers; // Array of numbers to check
    int start;    // Start index in the array
    int end;      // End index in the array (exclusive)
} PrimeWork;

bool isPrime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int i = 3; i <= sqrt(num); i += 2) {
        if (num % i == 0) return false;
    }
    return true;
}

void* countPrimes(void* arg) {
    PrimeWork* work = (PrimeWork*)arg;
    for (int i = work->start; i < work->end; i++) {
        if (isPrime(work->numbers[i])) {
            pthread_mutex_lock(&lock);
            primeCounter++;
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int main() {
    int number, size = 0, capacity = 10;
    int* numbers = (int*)malloc(capacity * sizeof(int));

    while (scanf("%d", &number) != EOF) {
        if (size >= capacity) {
            capacity *= 2;
            numbers = (int*)realloc(numbers, capacity * sizeof(int));
        }
        numbers[size++] = number;
    }

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    pthread_t threads[MAX_THREADS];
    PrimeWork works[MAX_THREADS];
    int chunkSize = size / MAX_THREADS;

    for (int i = 0; i < MAX_THREADS; i++) {
        works[i].numbers = numbers;
        works[i].start = i * chunkSize;
        works[i].end = (i == MAX_THREADS - 1) ? size : (i + 1) * chunkSize; // Last thread takes any remaining
        if (pthread_create(&threads[i], NULL, countPrimes, (void*)&works[i]) != 0) {
            perror("Failed to create thread");
            free(numbers);
            return 1;
        }
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d total primes.\n", primeCounter);

    pthread_mutex_destroy(&lock);
    free(numbers);

    return 0;
}

