#include "history.h"

void add_command(history *hs, const char *command)
{
   if (hs->count < HISTORY_MAX_SIZE) {
        hs->elements[(hs->count)++] = strdup( command );
   } else {
        free( hs->elements[0] );
        for (unsigned index = 1; index < HISTORY_MAX_SIZE; index++) {
            hs->elements[index - 1] = hs->elements[index];
        }
        hs->elements[HISTORY_MAX_SIZE - 1] = strdup( command );
    }
}

const char* get_elem(history *hs, unsigned index)
{
    return hs->elements[index];
}

const char* print_history(history *hs)
{
    
}