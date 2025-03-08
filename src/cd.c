/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:23:29 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 17:24:21 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_cd_error(char *msg, struct s_shell *shell)
{
	ft_putstr_fd(msg, 2);
	shell->exit_code = 1;
	return (1);
}

static char	*determine_path(char **argv, struct s_shell *shell)
{
	char	*path;
	char	*previous_dir;

	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		path = getenv("HOME");
		if (!path || !*path)
			return (NULL);
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		previous_dir = pop_dir(shell);
		if (!previous_dir)
			return (NULL);
		ft_printf("%s\n", previous_dir);
		path = previous_dir;
	}
	else
		path = argv[1];
	return (path);
}

int	ft_cd(char **argv, struct s_shell *shell, struct s_shell *head)
{
	char	cwd[1024];
	char	*path;
	char	*home;

	(void)head;
	ft_memset(cwd, 0, sizeof(cwd));
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (1);
	if (!argv[1])
	{
		home = getenv("HOME");
		if (!home)
			return (1);
		push_dir(shell, cwd);
		if (chdir(home) != 0)
		{
			perror("cd");
			return (1);
		}
		return (0);
	}
	path = determine_path(argv, shell);
	if (!path)
	{
		if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
			return (handle_cd_error("HOME not set\n", shell));
		else
			return (handle_cd_error("No previous directory\n", shell));
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		pop_dir(shell);
		return (1);
	}
	push_dir(shell, cwd);
	update_pwd();
	shell->exit_code = 0;
	return (0);
}
