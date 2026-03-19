#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"
#include <cjson/cJSON.h>

#include "../helpers/config/config.h"
#include "mqtt_queue/mqtt_queue.h"
#include "mqtt_event.h"

volatile int keepRunning = 1;

static bool handle_json(cJSON *json, const char *key, float *value);
static void on_temperature_received(float temperature);
static void handle_message(const char *topic_name, const char *message, size_t message_len);
static int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message);

static bool handle_json(cJSON *json, const char *key, float *value)
{
    cJSON *temp_item = cJSON_GetObjectItem(json, key);
    if (cJSON_IsNumber(temp_item))
    {
        *value = (float)temp_item->valuedouble;
        printf("Added temperature: %.2f\n", *value);
        return true;
    }

    fprintf(stderr, "Invalid or missing numeric field: %s\n", key);
    return false;
}

static void on_temperature_received(float temperature)
{
    mqtt_event_t event = {
        .type = EVENT_TEMPERATURE,
        .value = temperature};

    queue_push(event);
}

static void handle_message(const char *topic_name, const char *message, size_t message_len)
{
    cJSON *json = cJSON_ParseWithLength(message, message_len);
    if (json)
    {
        if (strcmp(topic_name, "sensor/temperature") == 0)
        {
            float temperature;

            if (handle_json(json, "temperature", &temperature))
            {
                on_temperature_received(temperature);
            }
        }
        else
        {
            printf("Received message: %.*s\n", (int)message_len, message);
        }

        cJSON_Delete(json);
    }
    else
    {
        printf("Failed to parse JSON\n");
    }
}

static int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    (void)context;
    (void)topicLen;

    printf("TAG: %s\n", topicName);
    printf("Message: %.*s\n", message->payloadlen, (char *)message->payload);

    handle_message(topicName, (const char *)message->payload, (size_t)message->payloadlen);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

int mqtt_init()
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    const char *ADDRESS = get_env_or_die("ADDRESS");
    const char *CLIENTID = get_env_or_die("CLIENTID");
    const char *TOPIC = get_env_or_die("TOPIC");
    const int QOS = atoi(get_env_or_die("QOS"));

    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);

    printf("Connect to MQTT...\n");
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Connection error: %d\n", rc);
        return -1;
    }

    printf("Connection succsess!\n: %s\n", TOPIC);

    MQTTClient_subscribe(client, TOPIC, QOS);

    while (keepRunning)
    {
        sleep(1);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}