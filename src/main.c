#include "mqtt_subscriber.h"
#include <pthread.h>
#include "mqtt_worker/mqtt_worker.h"

void *mqtt_thread(void *arg)
{
    mqtt_init();

    return NULL;
}

int main(void)
{
    pthread_t mqtt_thread_id;
    pthread_t worker_thread_id;

    pthread_create(&worker_thread_id, NULL, worker_thread, NULL);
    pthread_create(&mqtt_thread_id, NULL, mqtt_thread, NULL);

    pthread_join(mqtt_thread_id, NULL);
    return 0;
}
