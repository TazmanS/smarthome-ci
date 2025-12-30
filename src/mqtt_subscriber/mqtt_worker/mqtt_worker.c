#include <stdio.h>
#include "../mqtt_queue/mqtt_queue.h"
#include "mqtt_worker.h"
#include "../mqtt_stores/mqtt_temperature_store/mqtt_temperature_store.h"

void *worker_thread(void *arg)
{
    while (1)
    {
        mqtt_event_t event = queue_pop();

        switch (event.type)
        {
        case EVENT_TEMPERATURE:
            temperature_store_add(event.value);
            break;
        }
    }
    return NULL;
}
