#ifndef MAIN_H
#define MAIN_H
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#ifdef INIT_READ_SIZE
#undef INIT_READ_SIZE
#endif /* !INIT_READ_SIZE */
#define INIT_READ_SIZE 128

int handle_path(char *cmd, char *envp[]);
void child_process(char *cmd, char *args[], char *program_name);
void parse_cmd(char cmd[], char *args[], char *line);
void fork_process(int is_interactive, char *args[], char line[], char *program_name);
int check_exit(char *cmd);
int check_env(char *cmd, char **envp);
/* io utils */
unsigned int _strlen(char *str);
int _writestr(char *str);
ssize_t _getline(char **lineptr, size_t *n, int fd);
/* string utils */
int _strncmp(char *str1, char *str2, unsigned int n);
char *_strcat(char *dest, const char *src);
int _strcmp(char *s1, char *s2);
char *_strcpy(char *dest, const char *src);
char *_strtok(char *str, char *delim);

#endif /* !MAIN_H */
