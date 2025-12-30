#pragma once

typedef enum
{
    EVENT_TEMPERATURE
} event_type_t;

typedef struct
{
    event_type_t type;
    float value;
} mqtt_event_t;