#ifndef SH_BUILTINS_H
#define SH_BUILTINS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int set_prompt(char *arg);
char *get_prompt();

int sh_cd(char **args);
int sh_help(char **args);
int sh_exit(char **args);
int sh_chprmt(char **args);

int sh_num_builtins(void);

extern char *builtin_str[];
extern int (*builtin_func[])(char**);

#endif
