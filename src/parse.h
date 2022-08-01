#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RL_BUFSIZE 1024
#define TOK_BUFSIZE 64

char* read_line(void);

void print_words(char **words);

char **words(char *line, char* delim);

int find_index(char** argv, char* source);

int find_char(char* argv, char source);