/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:21:41 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/06 15:36:21 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_pwd(char **argv, struct s_shell *shell, struct s_shell *head)
{
	(void)argv;
	(void)head;
	char cwd[1024];
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

static t_dir_stack	*g_dir_stack = NULL;

void	push_dir(const char *dir)
{
	t_dir_stack	*new_node;

	new_node = malloc(sizeof(t_dir_stack));
	if (!new_node)
		return ;
	new_node->dir = strdup(dir);
	new_node->next = g_dir_stack;
	g_dir_stack = new_node;
}
char	*pop_dir(void)
{
	t_dir_stack *top;
	char *dir;

	if (!g_dir_stack)
		return (NULL);
	top = g_dir_stack;
	dir = top->dir;
	g_dir_stack = g_dir_stack->next;
	free(top);
	return (dir);
}

void clear_dir_stack(void)
{
    t_dir_stack *tmp;

    while (g_dir_stack)
    {
        tmp = g_dir_stack;
        g_dir_stack = g_dir_stack->next;
        free(tmp->dir);
        free(tmp);
    }
}
