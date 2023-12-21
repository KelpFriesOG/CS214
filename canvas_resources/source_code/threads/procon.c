#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "queue.h"

#define QSIZE
#define NUMP 2
#define NUMC 1
#define DELAYP 1
#define DELAYC 1
#define PAMOUNT 5

// this struct allows us to pass multiple parameters to each thread
// in principle, we could use this to configure each thread independently
// of the others
struct thconfig {
    int id;
    int amount;
    int delay;
};

queue_t global_queue;

void *producer(void *arg)
{
    int i, e;
    struct thconfig me = *(struct thconfig *) arg;

    for (i = 0; i < me.amount; i++) {
	e = q_enqueue(&global_queue, i);
	if (e) {
	    printf("%d: queue closed\n", me.id);
	    pthread_exit(NULL);
	}
	printf("%d: sent %d\n", me.id, i);
	sleep(me.delay);
    }

    return NULL;
}


void *consumer(void *arg)
{
    int i;
    struct thconfig me = *(struct thconfig *) arg;

    while (q_dequeue(&global_queue, &i) == 0) {
	printf("%d: Got %d\n", me.id, i);
	sleep(me.delay);
    }

    return NULL;
}

void *finite_consumer(void *arg)
{
    int i, j, e;
    struct thconfig me = *(struct thconfig *) arg;

    for (i = 0; i < me.amount; i++) {
	e = q_dequeue(&global_queue, &j);
	if (e) {
	    printf("%d: queue closed\n", me.id);
	    pthread_exit(NULL);
	}
	printf("%d: got %d\n", me.id, j);
	sleep(me.delay);
    }

    return NULL;
}


int main(int argc, char **argv)
{

    // exercise: rework this to make the number of threads dynamic
    pthread_t tids[NUMP + NUMC];
    struct thconfig configs[NUMP + NUMC];
    int i;


    q_init(&global_queue, QSIZE);

    for (i = 0; i < NUMP + NUMC; i++) {
	configs[i].id = i;
	configs[i].delay = i < NUMP ? DELAYP : DELAYC;
	configs[i].amount = PAMOUNT;
    }
    // because this array is fully initialized while the program is still
    // single-threaded, there should not be an issue with cache coherency:
    // all threads should see the initialized array, even if they are
    // scheduled on different CPUs


    for (i = 0; i < NUMP; i++) {
	pthread_create(&tids[i], NULL, producer, &configs[i]);
    }

    for (i = NUMP; i < NUMP + NUMC; i++) {
	pthread_create(&tids[i], NULL, consumer, &configs[i]);
    }

    printf("All threads created\n");

    for (i = 0; i < NUMP; i++) {
	pthread_join(tids[i], NULL);
    }

    printf("All producers finished\n");
    q_close(&global_queue);
    // Because the consumer(s) read an unlimited number of items, we can
    // close the queue once all the producers have completed.
    // What would need to change if the consumers accepted a finite number
    // of items?

    for (i = NUMP; i < NUMP + NUMC; i++) {
	pthread_join(tids[i], NULL);
    }

    return EXIT_SUCCESS;
    
}
