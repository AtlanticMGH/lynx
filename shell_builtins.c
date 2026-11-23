#include "shell_builtins.h"
#include <stdlib.h>

//builtin ideas:
// change appearance: changes '> ' to what user inputs

static char *sh_prompt = NULL;

//list of builtin commands followed by their functions
char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "chprmt"
};

int (*builtin_func[]) (char **) = {
    &sh_cd,
    &sh_help,
    &sh_exit,
    &sh_chprmt,
};

int sh_num_builtins(){
    return sizeof(builtin_str) / sizeof(char *);
}

//builtin function implementation
int sh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "sh: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("sh");
    }
  }
  return 1;
}

int sh_help(char **args){
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (int i = 0; i < sh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int sh_exit(char **args)
{
  return 0;
}

int sh_chprmt(char **args){
    if (args[1] == NULL) {
      fprintf(stderr, "sh: expected argument to \"chprmt\"\n");
    } else {
        //printf("%s\n",args[1]);
        set_prompt(args[1]);
    }
    return 1;
}

int set_prompt(char *arg){
    //get length of new prompt; +1 for '\0'
    size_t len = strlen(arg)+1;
    //change sh_prompt size to len and save in tmp
    char *tmp = realloc(sh_prompt, len);
    if(!tmp){
        perror("sh");
        return -1;
    }

    //copy memory from arg to tmp
    // set sh_prompt to tmp
    memcpy(tmp, arg, len);
    sh_prompt = tmp;
    return 1;
}

char *get_prompt(){
    return sh_prompt;
}
