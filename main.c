#include "shell_builtins.h"

void command_loop();
char *sh_read_line();
char **sh_split_line(char *line);
int sh_launch(char **args);
int sh_execute(char **args);
static char **push_token(char **tokens, int *count, int *bufsize, char *token);
char **tokenizer(char *string);
static bool is_delim(char c);

void print_tokens(char **tokens);
char **sh_split_line_experimental(char *line);

int main(){
    //load config

    //run command loop
    command_loop();
    //perform shutdown/cleanup


    return 1;
}



void command_loop(){
    //loop until exit command is given
    char *line;
    char **args;
    int status;

    do{
        printf("> ");
        //get user input
        //parse input
        //execute command
        line = sh_read_line();
        args = tokenizer(line);
        //print_tokens(args);
        status = sh_execute(args);

        free(line);
        free(args);
    } while(status);
}



#define RL_BUFSIZE 1024
char *sh_read_line(void){
    int bufsize = RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char)*bufsize);
    //int for checking for EOF
    int c;

    if(!buffer){
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        //read char
        c = getchar();

        //if EOF, replace with null character and return otherwise add char to string
        if(c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        //if buffer is exceeded, reallocate
        if(position>=bufsize){
            bufsize += RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            if(!buffer){
                fprintf(stderr, "sh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}



#define SH_TOK_BUFSIZE 64
#define SH_TOKEN_DELIM " \t\r\n\a"
char **sh_split_line(char *line){
    int bufsize = SH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if(!tokens){
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SH_TOKEN_DELIM);
    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position>=bufsize){
            bufsize += SH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if(!tokens){
                fprintf(stderr, "sh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SH_TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

void print_tokens(char **tokens){
    for(int i = 0; tokens[i] != NULL; i++){
        printf("tokens[%d] = \"%s\"\n", i, tokens[i]);
    }
}

static char **push_token(char **tokens, int *count, int *bufsize, char *token){
    tokens[(*count)++] = token;

    if(*count >= *bufsize){
        *bufsize += SH_TOK_BUFSIZE;
        char **tmp = realloc(tokens, *bufsize * sizeof(char *));
        if(!tmp){
            fprintf(stderr, "sh: allocation error\n");
            exit(EXIT_FAILURE);
        }
        tokens = tmp;
    }
    return tokens;
}

char **tokenizer(char *string){
    int bufsize = SH_TOK_BUFSIZE;
    int count = 0;
    bool in_quotes = false;
    char *token = NULL;

    char **tokens = malloc(bufsize * sizeof(char *));
    if(!tokens){
        fprintf(stderr, "sh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    for(char *p = string; *p != '\0'; p++){
        bool is_quote = (*p == '"');
        bool is_separator = in_quotes ? is_quote : (is_quote || is_delim(*p));

        if(is_separator){
            if(token){
                *p = '\0';
                tokens = push_token(tokens, &count, &bufsize, token);
                token = NULL;
            }

            in_quotes = is_quote && !in_quotes;

            if(in_quotes){
                token = p + 1;
            }
        }
        else if(!in_quotes && !token){
            token = p;
        }
    }

    if(in_quotes){
        fprintf(stderr, "sh: unclosed quote\n");
        free(tokens);
        return NULL;
    }

    if(token){
        tokens = push_token(tokens, &count, &bufsize, token);
    }

    tokens[count] = NULL;
    return tokens;
}

static bool is_delim(char c){
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}


int sh_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0){
        //child process
        if(execvp(args[0], args) == -1){
            perror("sh");
        }
        exit(EXIT_FAILURE);
    } else if(pid<0){
        //error forking
        perror("sh");
    } else {
      //parent process
      do {
          wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}



int sh_execute(char **args){
    if(args[0] == NULL){
        //empty commands was entered
        return 1;
    }

    for(int i = 0; i<sh_num_builtins(); i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }
    return sh_launch(args);
}
