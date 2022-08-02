#include "builtins.h"

char *strdup_b(const char *c)
{
    char *dup = malloc(strlen(c) + 1);
    if (dup != NULL)
        strcpy(dup, c);

    return dup;
}

void add_command_to_history(char *command)
{
    if (history_count < HISTORY_MAX_SIZE)
    {
        history[history_count++] = strdup_b(command);
    }
    else
    {
        free(history[0]);
        for (unsigned index = 1; index < HISTORY_MAX_SIZE; index++)
        {
            free(history[index - 1]);
            history[index - 1] = strdup_b(history[index]);
        }
        history[HISTORY_MAX_SIZE - 1] = strdup_b(command);
    }
}

int print_history(char **args)
{
    long index;
    if (args[1] == NULL)
    {
        for (unsigned i = 0; i < history_count; i++)
        {
            printf("%d %s\n", (i + 1), history[i]);
        }

        return 1;
    }
    else
    {
        char *end = NULL;
        index = strtol(args[1], &end, 10);
        printf(">%s\n", history[index - 1]);

        eval(history[index - 1]);

        return 1;
    }
}

int cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("builtin");
        }
    }
    return 1;
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv)
{
    if (!strcmp(argv[0], "quit")) /* quit command */
        exit(0);
    if (!strcmp(argv[0], "cd")) /* Ignore singleton & */
        return cd(argv);
    if (!strcmp(argv[0], "historico"))
        return print_history(argv);

    return 0; /* Not a builtin command */
}