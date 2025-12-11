#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MQTTClient.h"

#define ADDRESS     "tcp://192.168.31.210:1883"
#define CLIENTID    "RPI5_Client"
#define TOPIC       "sensor/temperature"
#define QOS         1

volatile int keepRunning = 1;

int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    printf("TAG: %s\n", topicName);
    printf("Message: %.*s\n", message->payloadlen, (char*)message->payload);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);

    printf("Connect to MQTT...\n");
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Connection error: %d\n", rc);
        return -1;
    }

    printf("Connection succsess!\n: %s\n", TOPIC);

    MQTTClient_subscribe(client, TOPIC, QOS);

    while (keepRunning) {
        sleep(1);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
