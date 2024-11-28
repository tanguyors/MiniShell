/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:53:09 by root              #+#    #+#             */
/*   Updated: 2024/11/28 10:50:47 by root             ###   ########.fr       */
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
    int i = 0;

    if (!array) 
    {
        return; // Rien à libérer
    }

    while (array[i]) 
    {
        free(array[i]); // Libère chaque chaîne de caractères
        i++;
    }
    free(array); // Libère le tableau lui-même
}
