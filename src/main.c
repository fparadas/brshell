#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jobs.h"

void parseline(char *buf, int *arg_count, char **argv);

int main () 
{
    int argc; // arguments count
    char *argv[128]; //argument list
    int bg, i = 0;


    signal (SIGINT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
    
    char cmdline[9999]; //change later

    printf("Parent pid: %d\n", getpid());

    fgets(cmdline, 9999, stdin);

    if (feof(stdin))
        exit(0);

    parseline(cmdline, &argc, argv);

    for (i=0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }
    
    return 1;

    // while (1)
    // {

    //     /* Read */
    //     parseline();
    //     eval();
    // }
}


/* parseline - Parse the command line and build the argv array */
void parseline(char *buf, int *arg_count, char **argv)
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
                buf++;
    }
    argv[argc] = NULL;

    *arg_count = argc;
}
