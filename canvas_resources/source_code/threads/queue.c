#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "queue.h"

#ifndef DEBUG
#define DEBUG 1
#endif


int q_init(queue_t *Q, unsigned capacity)
{
    // FIXME validate capacity

    Q->items = malloc(capacity * sizeof(data_t));
    Q->size = capacity;
    Q->length = 0;
    Q->head = 0;
    Q->closed = 0;
    pthread_mutex_init(&Q->lock, NULL);
    pthread_cond_init(&Q->enqueue_ready, NULL);
    pthread_cond_init(&Q->dequeue_ready, NULL);

    return 0;
}


// return 1 on failure, 0 on success
int q_enqueue(queue_t *Q, data_t item)
{
    pthread_mutex_lock(&Q->lock);

    if (Q->closed) {
	pthread_mutex_unlock(&Q->lock);
	return 1;
    }

    // check whether the queue is full
    while (Q->length == Q->size) {
	if (DEBUG) printf("Queue full; %d\n", Q->closed);
	if (Q->closed) {
	    pthread_mutex_unlock(&Q->lock);
	    return 1;
	}
	pthread_cond_wait(&Q->enqueue_ready, &Q->lock);
    }

    unsigned tail = Q->head + Q->length;
    if (tail >= Q->size) tail -= Q->size;

    Q->items[tail] = item;
    ++Q->length;

    pthread_cond_signal(&Q->dequeue_ready);

    if (DEBUG) printf("Wrote %d to %d; length  %d\n", item, tail, Q->length);
    pthread_mutex_unlock(&Q->lock);

    return 0;
}


int q_dequeue(queue_t *Q, data_t *ptr)
{
    pthread_mutex_lock(&Q->lock);
    
    // check whether queue is empty
    while (Q->length == 0) {
	if (Q->closed) {
	    pthread_mutex_unlock(&Q->lock);
	    return 1;
	}
	pthread_cond_wait(&Q->dequeue_ready, &Q->lock);
    }

    *ptr = Q->items[Q->head];
    --Q->length;
    ++Q->head;
    if (Q->head == Q->size) Q->head = 0;

    pthread_cond_signal(&Q->enqueue_ready);
    
    if (DEBUG) printf("Read %d; head=%d; length  %d\n", *ptr, Q->head, Q->length);
    pthread_mutex_unlock(&Q->lock);

    return 0;
}

int q_close(queue_t *Q)
{
    pthread_mutex_lock(&Q->lock);

    Q->closed = 1;
    pthread_cond_broadcast(&Q->dequeue_ready);
    pthread_cond_broadcast(&Q->enqueue_ready);

    pthread_mutex_unlock(&Q->lock);

    return 0;
}
