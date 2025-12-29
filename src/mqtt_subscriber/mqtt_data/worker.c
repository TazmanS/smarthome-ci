#include <stdio.h>
#include "mqtt_queue.h"
#include "worker.h"

#define MAX_TEMP_SIZE 10

static float temp_buffer[MAX_TEMP_SIZE];
static int temp_count = 0;

void process_temperature(float value)
{
    if (temp_count < MAX_TEMP_SIZE)
    {
        temp_buffer[temp_count++] = value;
    }

    if (temp_count == MAX_TEMP_SIZE)
    {
        float sum = 0.0f;
        for (int i = 0; i < MAX_TEMP_SIZE; i++)
            sum += temp_buffer[i];

        float avg = sum / MAX_TEMP_SIZE;
        printf("Average temperature: %.2f\n", avg);

        temp_count = 0;
    }
}

void *worker_thread(void *arg)
{
    while (1)
    {
        mqtt_event_t event = queue_pop();

        switch (event.type)
        {
        case EVENT_TEMPERATURE:
            process_temperature(event.value);
            break;

        case EVENT_LIGHT:
            printf("Light value: %.2f\n", event.value);
            break;
        }
    }
    return NULL;
}
