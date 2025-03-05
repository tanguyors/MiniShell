/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:21:41 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 22:21:42 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_pwd(char **argv, struct s_shell *shell, struct s_shell *head)
{
	(void)argv;
	char cwd[1024]; // Buffer pour stocker le chemin
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s\n", cwd);
		// Affiche le répertoire courant avec un saut de ligne
		shell->exit_code = 0;
		return (0);
	}
	else
	{
		perror("pwd"); // Affiche une erreur si getcwd échoue
		shell->exit_code = 22;
	}
}

static char	*construct_path(const char *base, const char *input)
{
	char	*full_path;
	size_t	base_len;

	base_len = ft_strlen(base);
	full_path = malloc(base_len + ft_strlen(input) + 2); // +2 pour '/' et '\0'
	if (!full_path)
		return (NULL);
	ft_strlcpy(full_path, base, base_len + 1);                    
		// Copie `base` dans `full_path`
	ft_strlcat(full_path, "/", base_len + ft_strlen(input) + 2);   // Ajoute '/'
	ft_strlcat(full_path, input, base_len + ft_strlen(input) + 2);
		// Ajoute `input`
	return (full_path);
}

static t_dir_stack	*g_dir_stack = NULL; // Pile globale

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
		return (NULL); // Pile vide
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
