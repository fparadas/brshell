#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jobs.h"
#include <sys/stat.h>
#include <fcntl.h>
#include "parse.h"

#define TOK_DELIM " \t\r\n\a"
#define PIPE_DELIM "|"

extern job* first_job;

process* make_process(char** argv, process* next);

job* create_job(char* cmdline);

void eval(char* line);