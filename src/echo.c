/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:23:14 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 17:24:35 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_echo(char **argv, struct s_shell *shell, struct s_shell *head)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	(void)head;
	i = 1;
	while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	shell->exit_code = 0;
	return (0);
}
