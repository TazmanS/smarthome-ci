#include <pthread.h>
#include "mqtt_subscriber/mqtt_subscriber.h"
#include "mqtt_subscriber/mqtt_worker/mqtt_worker.h"

void *mqtt_thread()
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
