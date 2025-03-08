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

static int	cd_home(struct s_shell *shell)
{
	char	*home;
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (1);
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

int	ft_cd(char **argv, struct s_shell *shell, struct s_shell *head)
{
	char	cwd[1024];

	(void)head;
	if (!argv[1])
		return (cd_home(shell));
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (1);
	push_dir(shell, cwd);
	if (chdir(argv[1]) != 0)
	{
		perror("cd");
		pop_dir(shell);
		return (1);
	}
	return (0);
}
