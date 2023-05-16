#ifndef MAIN_H
#define MAIN_H

int handle_path(char *cmd, char *envp[]);
void child_process(char *cmd, char *args[], char *program_name);
void parse_cmd(char cmd[], char *args[], char *line);
void fork_process(int is_interactive, char *args[], char line[], char *program_name);

int check_exit(char *cmd);
int check_env(char *cmd, char **envp);

unsigned int _strlen(char *str);
int _writestr(char *str);
int _strncmp(char *str1, char *str2, unsigned int n);
char *_strcat(char *dest, const char *src);
int _strcmp(char *s1, char *s2);

#endif /* !MAIN_H */
