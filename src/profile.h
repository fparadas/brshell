#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "strmap.h"

extern StrMap *sm;

void read_alias(char *f);

char* alias(char* str);