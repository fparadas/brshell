#include "parse.h"

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
        } else if (c == '#') {
            /* read and ignore the rest*/
            while (1) {
                c = getchar();
                if (c == '\n') {
                    buffer[index] = '\0';
                    return buffer;
                }
            }
        } else if (c == '\n') {
            buffer[index] = '\0';
            return buffer;
        } else {
            buffer[index] = c;
        }

        index++;

        if (index >= bufsize) {
            bufsize += RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if (!buffer) {
                fprintf(stderr, "allocation\n");
                exit(EXIT_FAILURE);
            }
        }
    }
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

char **words(char *line, char* delim)
{
    int bufsize = TOK_BUFSIZE, index = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *line_backup = malloc((strlen(line) + 1) * sizeof(char));
    char *token, **tokens_backup;

    if(!tokens || !line_backup) {
        fprintf(stderr, "allocation error");
        exit(EXIT_FAILURE);
    }
    strcpy(line_backup, line);

    token = strtok(line_backup, delim);
    while (token != NULL) {
        tokens[index] = token;
        index++;

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

    return tokens;
}

/* Simple lookup for vector of string */
int find_index(char** argv, char* source)
{
    int i = 0;
    char* word = argv[i];

    while (word != NULL) {
        if(strcmp(word, source) == 0)
            return i;
        
        word = argv[++i];
    }

    return -1;
}

int find_char(char* argv, char source) 
{
    int len = strlen(argv);

    for (int i = 0; i < len; i++) {
        if (argv[i] == source)
            return i;
    }

    return -1;
}