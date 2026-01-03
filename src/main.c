#include <pthread.h>
#include "mqtt_subscriber/mqtt_subscriber.h"
#include "mqtt_subscriber/mqtt_worker/mqtt_worker.h"
#include "data_base/data_base.h"

#include <libpq-fe.h>

void *mqtt_thread()
{
    mqtt_init();

    return NULL;
}

int main(void)
{
    PGconn *conn = data_base_init();

    pthread_t mqtt_thread_id;
    pthread_t mqtt_worker_thread_id;
    pthread_t data_base_thread_id;

    pthread_create(&mqtt_worker_thread_id, NULL, mqtt_worker_thread, NULL);
    pthread_create(&mqtt_thread_id, NULL, mqtt_thread, NULL);
    pthread_create(&data_base_thread_id, NULL, data_base_worker_thread, conn);

    pthread_join(mqtt_thread_id, NULL);
    pthread_join(data_base_thread_id, NULL);
    return 0;
}
