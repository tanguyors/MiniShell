/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:46:03 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 17:55:07 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void redir_input_exec(struct s_shell *shell, struct s_shell *head, int fd, int saved_stdin)
{
    if (head && head->token == TOKEN_CMD  && !shell->file)
        extract_data(shell, head);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    close(fd);	
}

/* Gestion de la redirection d'entrée (<)
	Redirige l'entrée standard vers un fichier 
	resultat < file affiché sur la console même avant un pipe ( a regler ) */
static void redir_input(struct s_shell *shell, struct s_shell *current)
{
    int fd;
	int saved_stdin;
	struct s_shell *head;

	head = current;
	fd = setup_redirection(shell, current, O_RDONLY, 0);
	if (fd == -1)
	{
		perror("open: ");
		shell->exit_code = 1;
		return ;
	}
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1)
    {
        close(fd);
        return ;
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        close(fd);
        close(saved_stdin);
        return ;
    }
	redir_input_exec(shell, head, fd, saved_stdin);
}

/* Gestion de la redirection de sortie (>)
	Redirige la sortie standard vers un fichier */
static void redir_output(struct s_shell *shell, struct s_shell *first_arg)
{
    int fd;
	int saved_stdout;
	struct s_shell *head;

	head = first_arg;
	fd = setup_redirection(shell, first_arg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return ;
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(fd);
        return ;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        close(saved_stdout);
        return ;
    }
    if (head && head->token == TOKEN_CMD && !shell->file)
        extract_data(shell, head);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(fd);
}

/* Gestion de la redirection en mode append (>>)
	Ajoute la sortie à la fin du fichier */
static void redir_append(struct s_shell *shell, struct s_shell *current)
{
    int fd;
	int saved_stdout;
	struct s_shell *head;

	head = current;
	fd = setup_redirection(shell, current, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return ;
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(fd);
        return ;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        close(saved_stdout);
        return ;
    }
    if (head && head->token == TOKEN_CMD && !shell->file)
        extract_data(shell, head);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    close(fd);
}

/* Redirige vers le bon token de redirection 
	SUPPR PRINTF erreur heredoc */
void redirection_execution(struct s_shell *shell, struct s_shell *first_arg)
{
	struct s_shell *which_redir;

	which_redir = first_arg;
	while (!is_token_red(which_redir->token))
		which_redir = which_redir->next;
	if (which_redir->token == REDIR_INPUT)
	{
		redir_input(shell, first_arg);
	}
	else if (which_redir->token == REDIR_OUTPUT)
	{
		redir_output(shell, first_arg);
	}
	else if (which_redir->token == REDIR_APPEND)
	{
		redir_append(shell, first_arg);
	}
	else if (which_redir->token == REDIR_HEREDOC)
	{
		redir_heredoc(shell, first_arg);
	}
}