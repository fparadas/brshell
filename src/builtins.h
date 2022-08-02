#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "eval.h"

#define HISTORY_MAX_SIZE 10

extern char *history[HISTORY_MAX_SIZE];
extern unsigned history_count;

void add_command_to_history( char *command );

int builtin_command(char **argv);