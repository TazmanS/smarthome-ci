#include <stdio.h>
#include <stdlib.h>

const char *get_env_or_die(const char *name)
{
    const char *value = getenv(name);

    if (!value)
    {
        fprintf(stderr, "Missing env var: %s\n", name);
        exit(1);
    }

    return value;
}
