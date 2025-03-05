/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:42:30 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 17:50:53 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void child_redir(struct s_shell *shell, struct s_shell *current)
{
	struct s_shell *current_redir;
	struct s_shell *first_arg;

	shell->file = current;
	current_redir = current;
	current_redir = current_redir->next;
	while (current)
	{
		if (is_redirection_in_list(current))
		{
			if (current_redir->next)
			{
				while(current_redir && current_redir->token != TOKEN_CMD)
				{
					current_redir = current_redir->next;
				}
				redirection_execution(shell, current_redir);
				current = current->next;
			}
		}
		current = current->next;
	}
}

void child_process(struct s_shell *shell, int fd[2], struct s_shell *current, struct s_shell *head)
{
	int nb_pipe;

	nb_pipe = is_pipe(current);
	if (shell->prev_fd != -1)
	{
		if (dup2(shell->prev_fd, STDIN_FILENO) < 0)
			exit_with_error("dup2 error prev_fd", NULL, 1);
		close(shell->prev_fd);
	}
	child_redir(shell, current);
	if (nb_pipe)
	{
		nb_pipe--;
		if (dup2(fd[1], STDOUT_FILENO) < 0)
			exit_with_error("dup2 error fd[1]", NULL, 1);
	}
	close(fd[0]);
	close(fd[1]);
	extract_data(shell, current);
	rl_clear_history();
	free(shell->rl_input);
	free_list(head);
	exit(shell->exit_code);
}