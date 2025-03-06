/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:14:25 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/06 15:44:35 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * free_array - Libère un tableau de chaînes de caractères.
 * @array: Le tableau à libérer.
 */
void free_array(char **array)
{
    int i;
    
    i = 0;
    if (!array) 
        return; // Rien à libérer
    while (array[i]) 
    {
        free(array[i]); // Libère chaque chaîne de caractères
        i++;
    }
    free(array); // Libère le tableau lui-même
}

void free_list(struct s_shell *head)
{
	struct s_shell	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		if (tmp->data != NULL)
			free(tmp->data);
		free(tmp);
	}
}

void free_stack(t_dir_stack *head)
{
	t_dir_stack *tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

/**
 * trim_newline - Supprime le '\n' à la fin de la chaîne, si présent.
 * @line: La chaîne à nettoyer.
 */
void trim_newline(char *line)
{
    size_t len;

    if (!line)
        return ;
    len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') 
    {
        line[len - 1] = '\0';
    }
}

void update_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)))
    {
        setenv("OLDPWD", getenv("PWD"), 1); // Met à jour OLDPWD avec l'ancien PWD
        setenv("PWD", cwd, 1);              // Met à jour PWD avec le chemin actuel
    }
}
