#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "data_base.h"
#include "../mqtt_subscriber/mqtt_stores/mqtt_temperature_store/mqtt_temperature_store.h"
#include "../helpers/config/config.h"

void insert_temperature(PGconn *conn, float value)
{
    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO temperature (value) VALUES (%.2f);", value);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Error to insert value: %s\n", PQerrorMessage(conn));
    }
    PQclear(res);
}

void *data_base_worker_thread(void *arg)
{
    PGconn *conn = (PGconn *)arg;
    const int DB_INTERVAL_SEC = atoi(get_env_or_die("DB_INTERVAL_SEC"));

    while (1)
    {
        float temp = temperature_store_avg();
        insert_temperature(conn, temp);
        sleep(DB_INTERVAL_SEC);
    }

    return NULL;
}

PGconn *data_base_init()
{
    const char *DB_HOST = get_env_or_die("DB_HOST");
    const char *DB_NAME = get_env_or_die("DB_NAME");
    const char *DB_USER = get_env_or_die("DB_USER");
    const char *DB_PASSWORD = get_env_or_die("DB_PASSWORD");
    const int DB_PORT = atoi(get_env_or_die("DB_PORT"));

    char conninfo[512];
    snprintf(conninfo, sizeof(conninfo),
             "host=%s port=%d dbname=%s user=%s password=%s",
             DB_HOST, DB_PORT, DB_NAME, DB_USER, DB_PASSWORD);

    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Error to connect to database: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    return conn;
}
