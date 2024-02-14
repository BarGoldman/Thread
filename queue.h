#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct node {
    int* numbers;  // Pointer to an array of numbers in the batch
    int count;     // The number of numbers in the batch
    void* data;
    struct node* next;  // Pointer to the next task in the queue
} queueNode;

typedef struct queue {
    queueNode* head; // Pointer to the front of the queue
    queueNode* tail; // Pointer to the end of the queue
    pthread_mutex_t lock; 
    pthread_cond_t cond; 
} myQueue;

void createQueue(myQueue *q);


#endif