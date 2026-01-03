#pragma once

#include <libpq-fe.h>

PGconn *data_base_init();
void *data_base_worker_thread(void *arg);