#include <pthread.h>

static float values[10];
static int count = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void temperature_store_add(float t)
{
    pthread_mutex_lock(&mutex);

    if (count < 10)
        values[count++] = t;
    else
    {
        memmove(&values[0], &values[1], sizeof(float) * 9);
        values[9] = t;
    }

    pthread_mutex_unlock(&mutex);
}

float temperature_store_avg(void)
{
    pthread_mutex_lock(&mutex);

    if (count == 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0.0f;
    }

    float sum = 0;
    for (int i = 0; i < count; i++)
        sum += values[i];

    pthread_mutex_unlock(&mutex);
    return sum / count;
}
