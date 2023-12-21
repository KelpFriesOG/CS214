
typedef int data_t;

typedef struct {
    data_t *items;
    unsigned size;    // length of items
    unsigned length;  // items in queue
    unsigned head;    // index of oldest element
    unsigned closed;  // whether we have closed
    pthread_mutex_t  lock;
    pthread_cond_t enqueue_ready;
    pthread_cond_t dequeue_ready;
} queue_t;


int q_init(queue_t *Q, unsigned capacity);
int q_enqueue(queue_t *Q, data_t item);
int q_dequeue(queue_t *Q, data_t *ptr);
int q_close(queue_t *Q);
