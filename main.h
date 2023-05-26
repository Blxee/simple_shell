#ifndef MAIN_H
#define MAIN_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

extern char **environ;

#ifdef MEM_ARRAY_SIZE
#undef MEM_ARRAY_SIZE
#endif /* !MEM_ARRAY_SIZE */
#define MEM_ARRAY_SIZE 256

#ifdef READ_BUFFER_SIZE
#undef READ_BUFFER_SIZE
#endif /* !READ_BUFFER_SIZE */
#define READ_BUFFER_SIZE 1024

#ifdef INIT_READ_SIZE
#undef INIT_READ_SIZE
#endif /* !INIT_READ_SIZE */
#define INIT_READ_SIZE 128
#define MAX_INPUT_LENGTH 100
#define MAX_ALIASES 10

/**
 * struct alias - data structor for alias command
 *
 * @name: name of the alias
 * @value: value of the alias
 */
typedef struct alias
{
	char *name;
	char *value;
} Alias;

/* handle  aliases */
int check_alias(char **args);
void set_alias(char **args);
Alias *create_alias(char *name, char *value);
void print_aliases(void);

/* shell utils */
int handle_exit(char **args);
int handle_path(char **cmd);
void child_process(char *cmd, char *args[]);
void parse_cmd(char *args[], char *line, int stdin_fd);
void fork_process(int is_interactive, char **args);
int check_custom_commands(char **args);
void get_quoted_strings(char **line, char **quoted_strings, int stdin_fd);
char **get_program_name(void);
void next_separator(char ***next_cmd, char *sep);
void expand_quote(char **str, char ***quotes);
void replace_variables(char **line);
int *get_last_cmd_exit(void);
int open_file(int argc, char **argv);

/* custom_commands */
char *_getenv(char *var);
char *_setenv(char *var, char *value);
int check_env(char *cmd);
int check_setenv(char **args);
int check_unsetenv(char **args);
int check_cd(char **args);
int check_alias(char **args);
void replace_aliased(char **cmd);

/* io utils */
int _writestr(int fd, char *str);
ssize_t _getline(char **lineptr, size_t *n, int fd);

/* string utils */
char *_strdup(const char *str);
int _atoi(char *s);
int _strncmp(char *str1, char *str2, unsigned int n);
char *_strcat(char *dest, const char *src);
int _strcmp(char *s1, char *s2);
char *_strcpy(char *dest, const char *src);
char *_strtok(char *str, char *delim);
unsigned int _strlen(const char *str);
int find_chars(char *str, char *chars);
int _isdigit(int c);
char *int_to_str(long n);
char *_strncpy(char *dest, const char *src, unsigned int n);

/* memory utils */
void *alloc_mem(unsigned long size);
void free_mem(void *ptr);
void free_all(void);

/* error messages */

#endif /* !MAIN_H */
