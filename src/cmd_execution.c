/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_execution.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:47:56 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/08 16:36:00 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	failed_execve(struct s_shell *current, struct s_shell *shell,
		char **data, char **args)
{
	if (ft_strchr(current->data, '/') && ft_strchr(current->data, '.'))
	{
		perror(" execve: ");
		rl_clear_history();
		clear_dir_stack();
		free(shell->rl_input);
		free(current);
		free_array(data);
		exit_with_error(NULL, args, 126);
	}
	else
	{
		ft_putstr_fd(" command not found\n", 2);
		rl_clear_history();
		clear_dir_stack();
		free(shell->rl_input);
		free(current);
		free_array(data);
		exit_with_error(NULL, args, 127);
	}
}

static void	std_exec_condition(struct s_shell *current, struct s_shell *shell,
		char **data)
{
	extern char	**environ;
	char		**args;
	char		*command;

	command = get_absolute_path(current->data);
	if (!command)
		command = current->data;
	args = get_all_data(current);
	if (execve(command, args, environ) == -1)
	{
		failed_execve(current, shell, data, args);
	}
	free_array(args);
}

static void	std_execution(struct s_shell *shell, struct s_shell *current,
		char **data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		exit_with_error("fork error", NULL, 1);
	else if (pid == 0)
	{
		std_exec_condition(current, shell, data);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_code = WTERMSIG(status) + 128;
	}
}

/* Initialise les builtin et parcourt l'array builtin afin de,
		trouver la commande correspondante
	si la commande reste introuver cela signifie qu'il s'agit,
		d'une commande système
	std_execution() sera donc appelé */
void	cmd_execution(struct s_shell *shell, struct s_shell *current,
		char **data)
{
	t_builtin	builtin[8];
	int			i;

	i = 0;
	initialize_builtin(builtin);
	while (builtin[i].name != NULL)
	{
		if (ft_strcmp(current->data, builtin[i].name) == 0)
		{
			builtin[i].func(data, shell, current);
			return ;
		}
		i++;
	}
	std_execution(shell, current, data);
}
