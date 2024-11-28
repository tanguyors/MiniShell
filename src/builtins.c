/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:51:41 by root              #+#    #+#             */
/*   Updated: 2024/11/28 11:09:00 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Implementations des commandes interne
#include "../include/minishell.h"

/**
 * ft_echo - Implémente le built-in echo.
 * @args: Tableau d'arguments, args[0] = "echo".
 *
 * Retourne 0 en cas de succès.
 */
int ft_echo(char **argv) 
{
    int i = 1; // Commence à analyser après "echo"
    int newline = 1; // Par défaut, ajoute un saut de ligne
    // Vérifie si l'option "-n" est présente
    if (argv[i] && ft_strcmp(argv[i], "-n") == 0) 
    {
        newline = 0; // Désactive le saut de ligne
        i++;
    }
    // Affiche les arguments restants
    while (argv[i]) 
    {
        ft_putstr_fd(argv[i], 1); // Utilise la libft pour écrire la chaîne
        if (argv[i + 1]) 
        {
            ft_putchar_fd(' ', 1); // Ajoute un espace entre les arguments
        }
        i++;
    }
    if (newline) 
    {
        ft_putchar_fd('\n', 1); // Ajoute un saut de ligne si nécessaire
    }
    return 0;
}


