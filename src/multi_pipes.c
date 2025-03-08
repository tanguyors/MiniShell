/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:40:04 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/08 15:28:45 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	pipe_and_fork(int fd[2], int *pid)
{
	if (pipe(fd) == -1)
		exit_with_error("pipe error", NULL, 1);
	*pid = fork();
	if (*pid < 0)
		exit_with_error("fork error", NULL, 1);
}

static void	m_p_h_fork_suceed(struct s_shell *current, struct s_shell *shell,
		int fd[2])
{
	if (shell->prev_fd != -1)
		close(shell->prev_fd);
	if (current->next)
	{
		close(fd[1]);
		shell->prev_fd = fd[0];
	}
	else
		close(fd[0]);
}

static void	handle_child_processes(struct s_shell *shell, struct s_shell *head,
		pid_t *pids, int *pid_count)
{
	int				fd[2];
	pid_t			pid;
	struct s_shell	*current;

	current = head;
	while (current)
	{
		pipe_and_fork(fd, &pid);
		if (pid == 0)
			child_process(shell, fd, current, head);
		else
		{
			pids[*pid_count] = pid;
			(*pid_count)++;
			m_p_h_fork_suceed(current, shell, fd);
		}
		current = current->next;
		while (current && current->token != TOKEN_CMD)
			current = current->next;
	}
}

static void	wait_for_processes(struct s_shell *shell, pid_t *pids,
		int pid_count)
{
	int	status;
	int	i;

	i = 0;
	while (i < pid_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == pid_count - 1)
		{
			if (WIFEXITED(status))
				shell->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_code = WTERMSIG(status) + 128;
		}
		i++;
	}
}

void	multi_pipe_handling(struct s_shell *shell, struct s_shell *head)
{
	pid_t	pids[1024];
	int		pid_count;

	pid_count = 0;
	shell->prev_fd = -1;
	handle_child_processes(shell, head, pids, &pid_count);
	wait_for_processes(shell, pids, pid_count);
}
