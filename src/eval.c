#include "eval.h"

process* make_process(char** argv, process* next) {
    process* p = malloc(sizeof(process));

    if (!p) {
        fprintf(stderr, "allocation error");
        exit(EXIT_FAILURE);
    }

    p->argv = argv;
    p->completed = 0;
    p->stopped = 0;
    p->next = next;

    return p;
}

job* create_job(char* cmdline)
{
    int result, foreground = 1;
    char **tokens, **commands, **command_tokens, *command, **parsedline;
    job* j = malloc(sizeof(job));
    process *p, *last;
    size_t n_commands = 0;

    if (!j) {
        fprintf(stderr, "allocation error");
        exit(EXIT_FAILURE);
    }

    tokens = words(cmdline, TOK_DELIM);

    if ((result = find_char(cmdline, '&')) != -1) {
        /* job will run in background */
        foreground = 0;
    }

    if ((result = find_index(tokens, ">")) != -1) {
        /* change stdout of job */
        j->stdout = open(tokens[result + 1], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    } else if ((result = find_index(tokens, ">>")) != -1 ) {
        /* change stdout of job */
        j->stdout = open(tokens[result + 1], O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);
    } else {
        j->stdout = STDOUT_FILENO;
    }

    if ((result = find_index(tokens, "<")) != -1) {
        /* change stdin of job */
        j->stdin = open(tokens[result + 1], O_RDONLY, S_IRWXU);
    } else {
        j->stdin = STDIN_FILENO;
    }

    j->stderr = STDERR_FILENO;

    j->notified = 0;
    j->pgid = 0;

    parsedline = words(cmdline, ">&<");
    
    /* divide into piped commands */

    commands = words(parsedline[0], PIPE_DELIM);

    /* get number of elements */
    command = commands[n_commands];
    while(command != NULL) {
        command = commands[++n_commands];
    }

    /* for each command on the pipe in the inverse order do: */
    last = NULL;
    for(int i = (n_commands - 1); i >= 0; i--) {
        command = commands[i];
        command_tokens = words(command, TOK_DELIM);
        p = make_process(command_tokens, last);
        last = p; 
    }

    j->first_process = p; /* set the current proccess as the first*/
    j->foreground = foreground;
    j->next = NULL;
    return j;
}

void eval(char* line)
{
    job* j;

    j = create_job(line);

    first_job = j;

    launch_job(j, j->foreground);
    do_job_notification();
}