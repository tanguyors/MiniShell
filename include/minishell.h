
#ifndef MINISHELL_H
# define MINISHELL_H

// Bibliothèques autorisées
# include <unistd.h>  // fork, execve, write, read, dup, dup2
# include <stdlib.h>  // malloc, free, getenv
# include <fcntl.h>   // open, O_CREAT, O_RDONLY
# include <errno.h>   // Gestion des erreurs (errno)
# include <sys/wait.h> // wait, waitpid
# include <signal.h>
# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

struct s_shell
{
    struct s_shell *next;
    struct s_shell *prev;
    enum tokens *token;
    char *data;
};

enum tokens
{
    TOKEN_WORD,
    TOKEN_CMD,
    TOEKEN_ARG,
    TOKEN_RED
};

/*-- Parsing --*/
char **parse_tokens(char *input);
void parsing(char *str, struct s_shell *value);
void parse_commands(char **tokens);
/*-- Utils --*/
void free_array(char **array);
void free_list(struct s_shell *head);
int ft_strcmp(const char *s1, const char *s2);
void trim_newline(char *line);
char *expand_var(char *str);
char *remove_quotes(char *str);
char *ft_strndup(const char *s, size_t n);
/*-- Built-in --*/
int ft_echo(char **argv);
int ft_pwd(void);
int ft_env(char **argv);
//int ft_cd(char **argv);
/*-- Signal --*/
void handle_signal(int sig, siginfo_t *info, void *context);


#endif
