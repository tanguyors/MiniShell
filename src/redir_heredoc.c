/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:43:41 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/06 15:38:29 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int setup_redirection(struct s_shell *shell, int flag, int file_access)
{
	int fd;

	if (!shell->file || !shell->file->next)
        return (-1);
	
    while (shell->file && shell->file->token != TOKEN_FILE)
	{
		shell->file = shell->file->next;
	}
	fd = open(shell->file->data, flag, file_access);
	if (fd == -1)
	{
		perror("open: ");
		shell->exit_code = 1;
		return (-1);
	}
	shell->file = shell->file->next;
	return(fd);
}

static void setup_heredoc(struct s_shell **current, int (*pipe_fd)[2])
{
	if (!*current || !(*current)->next)
        return ;
	while (*current && (*current)->next && (*current)->token != TOKEN_FILE)
		*current = (*current)->next;
    if (pipe(*pipe_fd) == -1)
        return ;
}

/* Permet de gérer le cas ou une variable d'environnement
	doit être interprêté */
char *h_expand_var(char *line, char **new_line, char **end_line)
{
	char *expanded_var;
	char *dollar_pos;
	char *before_dollar;
	char *after_dollar;

	dollar_pos = ft_strchr(line, '$');
	if (dollar_pos)
	{
		expanded_var = expand_variable(dollar_pos + 1);
		before_dollar = ft_substr(line, 0, dollar_pos - line);
		*new_line = ft_strjoin(before_dollar, expanded_var);
		free(before_dollar);
		while (*dollar_pos != ' ' && *dollar_pos)
			dollar_pos++;
		after_dollar = ft_substr(line, dollar_pos - line, expanded_var - line);
		*end_line = ft_strjoin(*new_line, after_dollar);
		free(line);
		free(after_dollar);
		line = *end_line;
		free(*new_line);
	}
	return (line);
}

static void loop_heredoc(struct s_shell *current, int (*pipe_fd)[2])
{
	char *line;
	char *new_line;
	char *end_line;
	size_t len;

    while (1)
    {
		write(1, "heredoc> ", 9);
        line = get_next_line(STDIN_FILENO);
		if (!line)
            break;
		line = h_expand_var(line, &new_line, &end_line);
        len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';
        if (ft_strcmp(line, current->data) == 0)
        {
            free(line);
            break;
        }
        write(pipe_fd[0][1], line, ft_strlen(line));
        write(pipe_fd[0][1], "\n", 1);
        free(line);
    }
}

/* Gestion du heredoc (<<)
	Lit l'entrée jusqu'à ce que le délimiteur soit rencontré */
void redir_heredoc(struct s_shell *shell, struct s_shell *current)
{
    int pipe_fd[2];
	int saved_stdin;
	struct s_shell *head;
	
	head = current;
	setup_heredoc(&current, &pipe_fd);
	if (*pipe_fd == -1)
		return ;
	loop_heredoc(current, &pipe_fd);
    saved_stdin = dup(STDIN_FILENO);
    close(pipe_fd[1]);
    if (dup2(pipe_fd[0], STDIN_FILENO) != -1)
    {
        if (head && head->token == TOKEN_CMD)
            extract_data(shell, head);
    }
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    close(pipe_fd[0]);
}
