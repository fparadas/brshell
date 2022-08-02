# BRshell

Autor: Felipe Paradas
Matrícula: 170009840

Uma simples shell com uma interface mais natural com os usuários que falam o português: BRsh.

Utiliza como referência [GNU Simple Shell Impl](https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html)

## Sintaxe dos comandos

Um comando consiste de um nome do comando seguido de um ou mais argumentos separados por espaço.

expr := command-name arg[1] arg[2] ... arg[n]

Além disso, um comando consiste de uma ou mais "pipe-section", sendo que cada uma é um comando simples separado por "|". O stdout do pipe-section[0] entra como o ultimo argumento do pipe-section[1] e o padrão se repete até n.

pipe-section[i] := command-name arg[1] arg[2] ... arg[n]
expr := pipe-section[0] | pipe-section[1] | ... | pipe-section[n]

### Ideias para o parser

O parser precisa primeiro identificar as pipe-sections e depois os comandos. cada pipe-section pode começar e terminar com um whitespace extra.

## Histórico

Existe um histórico (com 10 elemento) o qual pode ser lido e acessado. 

```C
#DEFINE HISTORY_MAX_SIZE 10
```

Para implementar o histórico utilizando vetores, foi necessário apenas de um vetor de strings e uma função que faz a rotação dos elementos toda vez que insere um novo.

```C
static const char *history[HISTORY_MAX_SIZE];
static const unsigned history_count = 0;

void add_command_to_history( const char *command )
{
   if (history_count < HISTORY_MAX_SIZE) {
        history[history_count++] = strdup( command );
   } else {
        free( history[0] );
        for (unsigned index = 1; index < HISTORY_MAX_SIZE; index++) {
            history[index - 1] = history[index];
        }
        history[HISTORY_MAX_SIZE - 1] = strdup( command );
    }
}
```



### Referencias:

https://brennan.io/2015/01/16/write-a-shell-in-c/;

https://www.youtube.com/watch?v=spD11nnLsLE

https://www.gnu.org/software/libc/manual/html_node/Implementing-a-Shell.html

http://pokristensson.com/strmap.html