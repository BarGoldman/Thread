
#include <pthread.h>
#include "queue.h"



void createQueue(myQueue *q) {


    q->head = NULL;
    q->tail = NULL;

	MUTEX_INIT(&q->lock);
	COND_INIT(&q->cond);
}

