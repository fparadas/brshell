#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jobs.h"

#define RL_BUFSIZE 1024
#define TOK_BUFSIZE 64


pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

/* The active jobs are linked into a list.  This is its head.   */
job *first_job = NULL;


char* read_line(void)
{
    int bufsize = RL_BUFSIZE;
    int index = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "allocation error");
        exit(EXIT_FAILURE);
    }

    while (1) {
        c = getchar();

        if (c == EOF) {
            exit(EXIT_SUCCESS);
        } else if (c == '\n') {
            buffer[index] = '\0';
            return buffer;
        } else {
            buffer[index] = c;
        }

        index++;

        if (index >= bufsize) {
            bufsize += RL_BUFSIZE;
            buffer = realloc(buffer, bufsize *sizeof(char));
            if (!buffer) {
                fprintf(stderr, "allocation\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **words(char *line, int* arg_count, char* delim)
{
    int bufsize = TOK_BUFSIZE, index = 0;
    int argc=1;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;

    if(!tokens) {
        fprintf(stderr, "allocation error");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, delim);
    while (token != NULL) {
        tokens[index] = token;
        index++;
        argc++;

        if(index >= bufsize) {
            bufsize += TOK_BUFSIZE;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "allocation error");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, delim);
    }

    tokens[index] = NULL;
    *arg_count = argc;
    return tokens;
}

void print_words(char **words)
{
    int i = 0;
    char *word = words[i];

    while (word != NULL) {
        printf ("%s\n", word);
        word = words[++i];
    }
}

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
}

job* make_job (char** args, int argc) 
{
    char* command;
    char** command_tokens;
    int index = 0;
    int tmp;
    job* j = malloc(sizeof(job));
    process *p, *last_process;

    last_process = NULL;
    if(!j) {
        fprintf(stderr, "allocation error");
        exit(EXIT_FAILURE);
    }
    
    /* for each command on the pipe do: */
    for(int i = (argc - 1); i >= 0; i--) {
        command = args[i];
        command_tokens = words(command, &tmp, " \t\r\n\a");
        p = make_process(command_tokens, last_process);
        last_process = p; 
    }

    j->first_process = p;

	j->stdin = STDIN_FILENO;
	j->stdout = STDOUT_FILENO;
	j->stderr = STDERR_FILENO;

	j->notified = 0;
	j->pgid = 0;

    return j;
}

int main ()
{
    char *line;
    char **commands, **bg_command;
    int status, argc, fg = 1;
    job *j;
    init_shell();

    do {
        printf("> ");
        line = read_line();
        bg_command = words(line, &argc, " \t\r\a\n");
        if(strcmp(bg_command[(argc-1)], "&")) 
            fg = 0;

        commands = words(line, &argc, "|");
        print_words(commands);
        j = make_job(commands, argc);

        first_job = j;

        launch_job(j, fg);
        do_job_notification();

        status = 1;
        // status = eval(j);
    } while (status);

    free(line);
    free(commands);
    return EXIT_SUCCESS;
}