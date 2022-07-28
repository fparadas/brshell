#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define HISTORY_MAX_SIZE 10

typedef struct history
{
    const char* elements[HISTORY_MAX_SIZE]; /* vector that contains the commands */
    unsigned count = 0; /* history current size, initialized at 0 */
} history;

void add_command(history*, const char*);

const char* get_elem(history*, unsigned);

const char* print_history(history*);