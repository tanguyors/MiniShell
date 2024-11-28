/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:51:09 by root              #+#    #+#             */
/*   Updated: 2024/11/28 10:56:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parsing des commandes 
#include "../include/minishell.h"

/**
 * parse_command - Découpe la ligne en tokens (arguments).
 * @input: La ligne brute entrée par l'utilisateur.
 *
 * Retourne un tableau de chaînes de caractères (les arguments de la commande).
 */
char **parse_command(char *input) 
{
    if (!input || *input == '\0') 
    {
        return NULL;
    }
    // Découpe la ligne avec ft_split, en séparant par les espaces
    char **tokens = ft_split(input, ' ');
    // Vérifie si la commande a été correctement découpée
    if (!tokens) 
    {
        perror("Parsing error"); // Affiche une erreur si la découpe échoue
    }
    return tokens; // Retourne le tableau de tokens
}
