#include "profile.h"

char* read_file(char* file_name)
{
    int bufsize = RL_BUFSIZE;
    int index = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;
    FILE *fp;
    if (!buffer) {
        fprintf(stderr, "allocation error");
        exit(EXIT_FAILURE);
    }

    fp = fopen(file_name, "r");

    while (!feof(fp)) {
        c = fgetc(fp);
        if (c != '"') {

            buffer[index] = c;

            index++;
        }
        
        if (index >= bufsize) {
            bufsize += RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "allocation\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(fp);
    printf("buffer: %s\n\n\n", buffer);
    return buffer;
}

void read_alias(char *file_name) 
{
    char **aliases, **alias_tokens, *alias, *file_content;
    int i = 0;

    file_content = read_file(file_name);
    aliases = words(file_content, "\n");

    alias = aliases[i];
    while(alias != NULL) {
        alias = aliases[++i];
    }

    sm = sm_new(i);

    for (int i = 0; (alias = aliases[i]) != NULL; i++) {
        alias_tokens = words(alias, " \t\r\n\a");
        sm_put(sm, alias_tokens[2], alias_tokens[1]);
    }
}

char* alias(char* str)
{
    char *buf = malloc(sizeof(char) * 255);
    int result = sm_get(sm, str, buf, 255);

    if (result == 0)
        return str;
    
    return buf;
}