#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"
#include <cjson/cJSON.h>

#include "config.h"
#include "mqtt_queue/mqtt_queue.h"
#include "mqtt_event.h"

volatile int keepRunning = 1;

float handle_json(cJSON *json, const char *key)
{
    cJSON *temp_item = cJSON_GetObjectItem(json, key);
    if (cJSON_IsNumber(temp_item))
    {
        float data = (float)temp_item->valuedouble;
        printf("Added temperature: %.2f\n", data);
        return data;
    }
};

void on_temperature_received(float temperature)
{
    mqtt_event_t event = {
        .type = EVENT_TEMPERATURE,
        .value = temperature};

    queue_push(event);
}

void handle_message(char *topic_name, MQTTClient_message *message)
{
    cJSON *json = cJSON_ParseWithLength(message->payload, message->payloadlen);
    if (json)
    {

        if (strcmp(topic_name, "sensor/temperature") == 0)
        {
            on_temperature_received(handle_json(json, "temperature"));
        }
        else
        {
            printf("Received message: %s\n", message);
        }

        cJSON_Delete(json);
    }
    else
    {
        printf("Failed to parse JSON\n");
    }
}

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("TAG: %s\n", topicName);
    printf("Message: %.*s\n", message->payloadlen, (char *)message->payload);

    handle_message(topicName, (char *)message->payload);

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
};