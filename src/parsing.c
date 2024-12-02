/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:51:09 by root              #+#    #+#             */
/*   Updated: 2024/12/02 15:48:20 by lmonsat          ###   ########.fr       */
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
    char **tokens;
    
    if (!input || *input == '\0') 
    {
        return (NULL);
    }
    // Découpe la ligne avec ft_split, en séparant par les espaces
    tokens = ft_split(input, ' ');
    // Vérifie si la commande a été correctement découpée
    if (!tokens) 
    {
        perror("Parsing error"); // Affiche une erreur si la découpe échoue
        exit(EXIT_FAILURE);
    }
    return (tokens); // Retourne le tableau de tokens
}
