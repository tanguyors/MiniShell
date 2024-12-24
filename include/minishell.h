
#ifndef MINISHELL_H
# define MINISHELL_H

// Bibliothèques autorisées
# include <unistd.h>  // fork, execve, write, read, dup, dup2
# include <stdlib.h>  // malloc, free, getenv
# include <fcntl.h>   // open, O_CREAT, O_RDONLY
# include <errno.h>   // Gestion des erreurs (errno)
# include <sys/wait.h> // wait, waitpid
# include <limits.h>
# include <signal.h>
# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# define ARG_MAX 2097152 // Arg max de la plupart des systèmes linux // getconf ARG_MAX
# define NAME_MAX 255 // Name max d'un nom de fichier // getconf NAME_MAX /
# define B_BLUE "\033[1;36m"
# define RESET "\033[0m"

enum e_tokens
{
    TOKEN_UNDEFINED, 
    TOKEN_CMD,
    TOKEN_ARG,
    TOKEN_RED,
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_APPEND,
    REDIR_HEREDOC,
	TOKEN_PIPE,
    TOKEN_FILE,
	TOKEN_SIMPLE_QUOTE,
	TOKEN_DOUBLE_QUOTE
};

struct s_shell
{
    struct s_shell *next;
    struct s_shell *prev;
    enum e_tokens token;
    char *data;
};

typedef struct s_dir_stack
{
    char                *dir; // Répertoire sauvegardé
    struct s_dir_stack  *next;
}   t_dir_stack;

//gestion et execution des builtins 
typedef struct s_builtin
{
    char *name;
    int (*func)(char **);
} t_builtin;


/*-- Parsing --*/
char **parse_tokens(char *input);
struct s_shell *parsing(char *str, struct s_shell *value);
void parse_commands(char **tokens);
int p_command(int *i, char *str, struct s_shell **value, int *stop_flag);
/*-- Linked_list --*/
void print_list(struct s_shell *current);
void print_token(struct s_shell *current);
struct s_shell	*create_node(char *data, char *token_type);
void insert_head(struct s_shell **head, char *new_data, char *token_type);
void insert_tail(struct s_shell **head, char *new_data, char *token_type);
struct s_shell *get_last_node(struct s_shell *head);
/*-- Utils --*/
void free_array(char **array);
void free_list(struct s_shell *head);
int ft_strcmp(const char *s1, const char *s2);
char *ft_strcpy(char *dest, char *src);
void trim_newline(char *line);
char *expand_var(char *str);
char *remove_quotes(char *str);
char *ft_strndup(const char *s, size_t n);
void update_pwd(void);

/*-- Is_Utils --*/
int is_spec_char(int c);
int is_ignored_char(int c);
int is_alpha(int c);
int is_redirect(int c);
int is_alnum(int c);
int is_space(int c);
/*-- Built-in --*/
int ft_echo(char **argv);
int ft_pwd(void);
int ft_env(char **argv);
int	ft_export(char **argv);
int	is_valid_identifier(const char *str);
int ft_unset(char **argv);
int ft_cd(char **argv);
static char *construct_path(const char *base, const char *input);
void push_dir(const char *dir);
char *pop_dir(void);
/*-- Signal --*/
void handle_signal(int sig, siginfo_t *info, void *context);


#endif
