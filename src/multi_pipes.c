/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:40:04 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 17:50:57 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void pipe_and_fork(int fd[2], int *pid)
{
	if (pipe(fd) == -1)
		exit_with_error("pipe error", NULL, 1);
	*pid = fork();
	if (*pid < 0)
		exit_with_error("fork error", NULL, 1);
}

static void m_p_h_wait(struct s_shell *shell, int last_pid, int pid)
{
	int status;

	waitpid(last_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	//else if (WIFSIGNALED(status))
		//shell->exit_code = WTERMSIG(status) + 128;
}

static void m_p_h_fork_suceed(struct s_shell *current, int prev_fd, int fd[2])
{
	if (prev_fd != -1)
		close(prev_fd);
	if (current->next)
	{
		close(fd[1]);
		prev_fd = fd[0];
	}
	else
		close(fd[0]);
}

/* Permet de gérer first_arg cas où un pipe est présent dans la liste. 
	Utilisation de fork afin de créer un processus enfant, 
	celui ci va redirigé la sortie de la commande en fonction des pipes. 
	Ici le double pointeur current représente la liste chaînée complète */
void multi_pipe_handling(struct s_shell *shell, struct s_shell *head)
{
    int fd[2];
    pid_t pid;
    pid_t last_pid;
	struct s_shell *current;

	last_pid = -1;
    shell->prev_fd = -1;
	current = head;
    while (current)
    {
        pipe_and_fork(fd, &pid);
        if (pid == 0)
			child_process(shell, fd, current, head);
        else
        {
            last_pid = pid;
			m_p_h_fork_suceed(current, shell->prev_fd, fd);
        }
        current = current->next;
        while (current && current->token != TOKEN_CMD)
            current = current->next;
    }
	if (last_pid != -1)
		m_p_h_wait(shell, last_pid, pid);
}