/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:53:09 by root              #+#    #+#             */
/*   Updated: 2024/12/02 15:49:14 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//fonctions utilitaire

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

int ft_strcmp(const char *s1, const char *s2) 
{
    while (*s1 && (*s1 == *s2)) 
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
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