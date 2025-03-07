/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 19:01:21 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 19:01:40 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Bibliothèques autorisées
# include "libft/libft.h"
# include <errno.h> // Gestion des erreurs (errno)
# include <fcntl.h> // open, O_CREAT, O_RDONLY
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>     // malloc, free, getenv
# include <sys/wait.h>   // wait, waitpid
# include <unistd.h>     // fork, execve, write, read, dup, dup2
# define ARG_MAX 2097152 // Arg max de la plupart des systèmes linux
	// getconf ARG_MAX
# define NAME_MAX 255    // Name max d'un nom de fichier // getconf NAME_MAX /
# define B_BLUE "\033[1;36m"
# define RESET "\033[0m"

enum					e_tokens
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

struct					s_shell
{
	struct s_shell		*next;
	struct s_shell		*prev;
	struct s_shell		*file;
	enum e_tokens		token;
	char				*data;
	int					prev_fd;
	int					exit_code;
	char				*rl_input;
	size_t				expand_input_len;
	size_t				expand_max_len;
	char				*expand_result;
};

typedef struct s_dir_stack
{
	char				*dir;
	struct s_dir_stack	*next;
}						t_dir_stack;

typedef struct s_builtin
{
	char				*name;
	int					(*func)(char **, struct s_shell *, struct s_shell *);
}						t_builtin;

/*-- Parsing --*/
char					**parse_tokens(char *input);
struct s_shell			*parsing(char *str, struct s_shell *head,
							struct s_shell *shell);
void					parse_commands(char **tokens);
int						p_redirection(int *i, char *str, struct s_shell **head,
							int *stop_flag);
int						p_quotes(int *i, char *str, struct s_shell **head);
int						p_arg(int *i, char *str, struct s_shell **head);
int						p_pipe(int *i, char *str, struct s_shell **head);
enum e_tokens			which_red(int *i, char *str);
/*-- Linked_list --*/
void					print_list(struct s_shell *current);
void					print_token(struct s_shell *current);
struct s_shell			*create_node(char *data, char *token_type);
void					insert_head(struct s_shell **head, char *new_data,
							char *token_type);
void					insert_tail(struct s_shell **head, char *new_data,
							char *token_type);
struct s_shell			*get_last_node(struct s_shell *head);
/*-- Utils --*/
void					free_array(char **array);
void					free_list(struct s_shell *head);
int						ft_strcmp(const char *s1, const char *s2);
char					*ft_strcpy(char *dest, char *src);
void					trim_newline(char *line);
char					*expand_var(char *str);
char					*remove_quotes(char *str);
char					*ft_strndup(const char *s, size_t n);
void					update_pwd(void);
const char				*get_token_name(enum e_tokens token);
void					exit_with_error(const char *str_error, char **array,
							int exit_code);
char					**get_all_data(struct s_shell *current);
char					**get_arg_data(struct s_shell *current,
							struct s_shell *shell);
char					*ft_strtok(char *str, const char *delim);
char					*ft_strncpy(char *dest, const char *src, size_t n);
char					*ft_strcat(char *dest, const char *src);
char					*expand_variable(const char *var);
int						get_nb_token(struct s_shell *current);
void					free_stack(t_dir_stack *head);
/*-- Is_Utils --*/
int						is_spec_char(int c);
int						is_ignored_char(int c);
int						is_alpha(int c);
int						is_str(char *str);
int						is_redirect(int c);
int						is_alnum(int c);
int						is_space(int c);
int						is_token_red(enum e_tokens token);
int						is_redirection_in_list(struct s_shell *head);
int						is_pipe(struct s_shell *current);
int						is_quotes(int c);
int						is_spec_char_no_space(int c);
char					*expand_token(const char *input, int is_in_single_quote,
							struct s_shell *shell);
/*-- Executions --*/
void					main_2(struct s_shell shell);
void					parse_execution(struct s_shell *shell,
							struct s_shell *head);
void					cmd_execution(struct s_shell *shell,
							struct s_shell *current, char **data);
void					extract_data(struct s_shell *shell,
							struct s_shell *current);
char					**get_all_data(struct s_shell *current);
char					**get_arg_data(struct s_shell *current,
							struct s_shell *shell);
char					*get_absolute_path(char *command);
/*-- Redirections --*/
void					redirection_execution(struct s_shell *shell,
							struct s_shell *first_arg);
void					redir_heredoc(struct s_shell *shell,
							struct s_shell *current);
int						setup_redirection(struct s_shell *shell, int flag,
							int file_access);
/*-- Pipes --*/
void					multi_pipe_handling(struct s_shell *shell,
							struct s_shell *head);
void					child_process(struct s_shell *shell, int fd[2],
							struct s_shell *current, struct s_shell *head);
/*-- Built-in --*/
void					initialize_builtin(t_builtin *builtin);
int						ft_exit(char **argv, struct s_shell *shell,
							struct s_shell *head);
int						ft_echo(char **argv, struct s_shell *shell,
							struct s_shell *head);
int						ft_pwd(char **argv, struct s_shell *shell,
							struct s_shell *head);
int						ft_unset(char **argv, struct s_shell *shell,
							struct s_shell *head);
int						ft_export(char **argv, struct s_shell *shell,
							struct s_shell *head);
int						is_valid_identifier(const char *str);
int						ft_unset(char **argv, struct s_shell *shell,
							struct s_shell *head);
int						ft_cd(char **argv, struct s_shell *shell,
							struct s_shell *head);
void					push_dir(const char *dir);
char					*pop_dir(void);
int						ft_env(char **argv, struct s_shell *shell,
							struct s_shell *head);
int						ft_export(char **argv, struct s_shell *shell,
							struct s_shell *head);
/*-- Signal --*/
void					handle_signal(int sig, siginfo_t *info, void *context);
void					clear_dir_stack(void);

#endif
