/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:23:07 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 19:04:27 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_env(char **argv, struct s_shell *shell, struct s_shell *head)
{
	int			i;
	extern char	**environ;

	(void)head;
	i = 0;
	if (argv[1])
	{
		ft_putstr_fd(" No such file or directory\n", 2);
		shell->exit_code = 1;
		return (1);
	}
	while (environ[i])
	{
		ft_printf("%s\n", environ[i]);
		i++;
	}
	shell->exit_code = 0;
	return (0);
}

static void	remove_env_var(const char *var)
{
	extern char	**environ;
	int			i;
	int			k;

	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], var, ft_strlen(var)) == 0
			&& environ[i][ft_strlen(var)] == '=')
		{
			k = i;
			while (environ[k + 1])
			{
				environ[k] = environ[k + 1];
				k++;
			}
			environ[k] = NULL;
			return ;
		}
		i++;
	}
}

int	ft_unset(char **argv, struct s_shell *shell, struct s_shell *head)
{
	int	i;

	(void)head;
	i = 1;
	if (!argv[0])
		return (0);
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd(" not a valid identifier\n", 2);
			shell->exit_code = 1;
			return (1);
		}
		else
			remove_env_var(argv[i]);
		i++;
	}
	shell->exit_code = 0;
	return (0);
}
