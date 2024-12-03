
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

//_______________________________________________________________________________________________________________________________//
                        // GESTION VARIABLE ENVIRONNEMENT//

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

/**
 * ft_export - Implémente la commande export.
 * @argv: Les arguments passés à la commande (argv[0] = "export").
 *
 * Retourne 0 en cas de succès, 1 en cas d'erreur.
 */
/**
 * is_valid_identifier - Vérifie si le nom de variable est valide.
 * @str: Le nom à vérifier.
 *
 * Retourne 1 si le nom est valide, sinon 0.
 */
int	is_valid_identifier(const char *str)
{
	int	i;

	// Vérifie que le premier caractère est une lettre ou un '_'
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	// Parcourt le reste de la chaîne pour vérifier les caractères autorisés
	i = 1;
	while (str[i] && str[i] != '=') // Arrête si un '=' est rencontré
	{
		if (!ft_isalnum(str[i]) && str[i] != '_') // Vérifie chaque caractère
			return (0); // Renvoie 0 si un caractère invalide est trouvé
		i++;
	}
	return (1); // Renvoie 1 si tout est valide
}

/**
 * ft_export - Implémente la commande export.
 * @argv: Les arguments passés à la commande (argv[0] = "export").
 *
 * Retourne 0 en cas de succès, 1 en cas d'erreur.
 */
int	ft_export(char **argv)
{
	int		i;      // Index pour parcourir les arguments
	char	*name;   // Stocke le nom de la variable
	char	*value;  // Stocke la valeur associée
	// Si aucun argument n'est donné, affiche toutes les variables exportées
	if (!argv[1])
	{
		i = 0;
		// Parcourt toutes les variables d'environnement
		while (environ[i])
		{
			// Affiche chaque variable avec le format "declare -x NOM=VALEUR"
			ft_printf("declare -x %s\n", environ[i]);
			i++;
		}
		return (0); // Fin de la commande avec succès
	}
	// Parcourt les arguments donnés pour les ajouter/modifier
	i = 1;
	while (argv[i])
	{
		// Vérifie si l'argument est un identifiant valide
		if (!is_valid_identifier(argv[i]))
		{
			// Affiche une erreur si l'identifiant est invalide
			ft_printf("export: `%s': not a valid identifier\n", argv[i]);
		}
		else
		{
			// Copie l'argument pour pouvoir le manipuler
			name = ft_strdup(argv[i]);

			// Cherche la position de '=' dans la chaîne
			value = ft_strchr(name, '=');
			if (value) // Si '=' est trouvé
			{
				*value = '\0';    // Coupe la chaîne en deux à '='
				value++;          // Pointe sur la valeur après '='
				setenv(name, value, 1); // Ajoute ou met à jour la variable dans environ
			}
			else
			{
				// Si aucune valeur n'est donnée, ajoute une variable avec une valeur vide
				setenv(name, "", 1);
			}
			free(name); // Libère la mémoire allouée pour le nom
		}
		i++; // Passe au prochain argument
	}
	return (0); // Fin de la commande avec succès
}
