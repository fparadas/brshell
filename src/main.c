#define _GNU_SOURCE

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "jobs.h"
#include "parse.h"
#include "eval.h"


pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

/* The active jobs are linked into a list.  This is its head.   */
job *first_job = NULL;


int main ()
{
    char *line, *user, *path;
    int status = 1;
    init_shell();

    do {
        path = get_current_dir_name();
        user = getlogin();
        printf("%s-%s> ", user, path);
        line = read_line();
        if(strlen(line) > 0)
            eval(line);
        // status = eval(j);
    } while (status);

    free(line);
    free(path);
    free(user);

    return EXIT_SUCCESS;
}