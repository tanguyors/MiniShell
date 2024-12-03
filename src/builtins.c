
//Implementations des commandes interne
#include "../include/minishell.h"

/**
 * ft_echo - Implémente le built-in echo.
 * @argv: Tableau d'arguments, argv[0] = "echo".
 *
 * Retourne 0 en cas de succès.
 */

int ft_echo(char **argv) 
{
    int i;
    int newline;

    i = 1; // Commence à analyser après "echo"
    newline = 1; // Par défaut, ajoute un saut de ligne
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
    return (0);
}

int ft_pwd(void) 
{
    char cwd[1024]; // Buffer pour stocker le chemin

    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        ft_printf("%s\n", cwd); // Affiche le répertoire courant avec un saut de ligne
        return (0);
    } 
    else 
    {
        perror("pwd"); // Affiche une erreur si getcwd échoue
        return (1);
    }
}

extern char **environ; // Déclare la variable globale environ qui se trouve dans le systeme 

/**
 * ft_env - Affiche toutes les variables d'environnement.
 * @argv: Les arguments passés à la commande (argv[0] = "env").
 *
 * Retourne 0 en cas de succès, 1 si des arguments sont passés.
 */
int ft_env(char **argv) 
{
    int i = 0;

    // Vérifie si des arguments supplémentaires sont donnés
    if (argv[1]) 
    {
        ft_printf("env: No such file or directory\n");
        return (1);
    }
    // Parcourt la variable globale environ et affiche chaque variable
    while (environ[i]) 
    {
        ft_printf("%s\n", environ[i]);
        i++;
    }
    return (0);
}
