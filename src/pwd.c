/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:21:41 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 20:28:32 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_pwd(char **argv, struct s_shell *shell, struct s_shell *head)
{
	char	cwd[1024];

	(void)argv;
	(void)head;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s\n", cwd);
		shell->exit_code = 0;
	}
	else
	{
		perror("pwd");
		shell->exit_code = 22;
	}
	return (0);
}

void	push_dir(struct s_shell *shell, const char *dir)
{
	t_dir_stack	*new;

	if (!shell || !dir)
		return;
	new = malloc(sizeof(t_dir_stack));
	if (!new)
		return;
	new->dir = NULL;
	new->next = NULL;
	new->dir = ft_strdup(dir);
	if (!new->dir)
	{
		free(new);
		return;
	}
	new->next = shell->dir_stack;
	shell->dir_stack = new;
}

char	*pop_dir(struct s_shell *shell)
{
	t_dir_stack	*top;
	char		*dir;

	if (!shell->dir_stack)
		return (NULL);
	top = shell->dir_stack;
	dir = top->dir;
	shell->dir_stack = shell->dir_stack->next;
	free(top);
	return (dir);
}

void	clear_dir_stack(struct s_shell *shell)
{
	t_dir_stack	*tmp;

	if (!shell || !shell->dir_stack)
		return;
	while (shell->dir_stack)
	{
		tmp = shell->dir_stack;
		shell->dir_stack = shell->dir_stack->next;
		free(tmp->dir);
		free(tmp);
	}
}
