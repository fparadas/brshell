#pragma once

#include <termios.h>
/* A process is a single process.  */
typedef struct process
{
  struct process *next;       /* next process in pipeline */
  char **argv;                /* for exec */
  pid_t pid;                  /* process ID */
  char completed;             /* true if process has completed */
  char stopped;               /* true if process has stopped */
  int status;                 /* reported status value */
} process;

/* A job is a pipeline of processes.  */
typedef struct job
{
  struct job *next;           /* next active job */
  char *command;              /* command line, used for messages */
  process *first_process;     /* list of processes in this job */
  pid_t pgid;                 /* process group ID */
  char notified;              /* true if user told about stopped job */
  struct termios tmodes;      /* saved terminal modes */
  int stdin, stdout, stderr;  /* standard i/o channels */
} job;


void free_job (job*);

void free_process (process*);

void print_job (job*);

void print_process (process*);

void launch_process (process *p, pid_t pgid,
                int infile, int outfile, int errfile,
                int foreground);


void launch_job (job *j, int foreground);

void do_job_notification (void);

void init_shell();