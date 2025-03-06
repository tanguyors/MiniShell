/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:23:29 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/06 15:37:03 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_cd_error(char *msg, struct s_shell *shell)
{
	ft_putstr_fd(msg, 2);
	shell->exit_code = 1;
	return (1);
}

static char	*determine_path(char **argv)
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
		previous_dir = pop_dir();
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

	(void)head;
	if (!getcwd(cwd, sizeof(cwd)))
		return (handle_cd_error("cd: error getting current directory\n", shell));
	path = determine_path(argv);
	if (!path)
	{
		if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
			return (handle_cd_error("HOME not set\n", shell));
		else
			return (handle_cd_error("No previous directory\n", shell));
	}
	if (chdir(path) == -1)
		return (handle_cd_error("No such file or directory\n", shell));
	push_dir(cwd);
	update_pwd();
	shell->exit_code = 0;
	return (0);
}
