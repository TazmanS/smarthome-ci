#pragma once

#include "mqtt_event.h"

void queue_init(void);
void queue_push(mqtt_event_t event);
mqtt_event_t queue_pop(void);
