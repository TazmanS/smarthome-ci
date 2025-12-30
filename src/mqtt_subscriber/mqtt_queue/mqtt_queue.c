#include "mqtt_queue.h"
#include <pthread.h>

#define QUEUE_SIZE 16

static mqtt_event_t queue[QUEUE_SIZE];
static int head = 0;
static int tail = 0;
static int count = 0;

static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

void queue_push(mqtt_event_t event)
{
    pthread_mutex_lock(&queue_mutex);

    if (count < QUEUE_SIZE)
    {
        queue[tail] = event;
        tail = (tail + 1) % QUEUE_SIZE;
        count++;
        pthread_cond_signal(&queue_cond);
    }

    pthread_mutex_unlock(&queue_mutex);
}

mqtt_event_t queue_pop(void)
{
    pthread_mutex_lock(&queue_mutex);

    while (count == 0)
    {
        pthread_cond_wait(&queue_cond, &queue_mutex);
    }

    mqtt_event_t event = queue[head];
    head = (head + 1) % QUEUE_SIZE;
    count--;

    pthread_mutex_unlock(&queue_mutex);
    return event;
}
