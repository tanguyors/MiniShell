
//Implementations des commandes interne
#include "../include/minishell.h"
//------------------------------------------------------------------------------------------ECHO----------------------------------------------------------------
/**
 * ft_echo - Implémente le built-in echo.
 * @argv: Tableau d'arguments, argv[0] = "echo".
 *
 * Retourne 0 en cas de succès.
 */

int ft_echo(char **argv)
{
    int i = 1;
    int newline = 1;

    // Gestion des multiples "-n"
    while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    // Affichage des arguments
    while (argv[i])
    {
        ft_putstr_fd(argv[i], 1);
        if (argv[i + 1])
            ft_putchar_fd(' ', 1);
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', 1);
    return (0);
}
 //--------------------------------------------------------------------------------------PWD------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------ENV---------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------EXPORT------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------------------------UNSET---------------------------------------------------------------------------------------------
// Version definitive a revoir pour mettre a la norme !!!!!
int	ft_unset(char **argv)
{
	int	i;
	int	j;
	int	k;

	if (!argv[1]) // Aucun argument : rien à faire
		return (0);

	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_printf("unset: `%s': not a valid identifier\n", argv[i]);
		}
		else
		{
			j = 0;
			while (environ[j])
			{
				// Compare uniquement la partie avant le '='
				if (ft_strncmp(environ[j], argv[i], ft_strlen(argv[i])) == 0 &&
					environ[j][ft_strlen(argv[i])] == '=')
				{
					// Décale toutes les entrées après j
					k = j;
					while (environ[k + 1])
					{
						environ[k] = environ[k + 1];
						k++;
					}
					environ[k] = NULL; // Termine le tableau après décalage
					break; // Une fois trouvée et supprimée, arrête la recherche
				}
				j++;
			}
		}
		i++;
	}
	return (0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------CD---------------------------------------------------------------------------------------------------------------
static char *construct_path(const char *base, const char *input)
{
    char *full_path;
    size_t base_len;

    base_len = ft_strlen(base);
    full_path = malloc(base_len + ft_strlen(input) + 2); // +2 pour '/' et '\0'
    if (!full_path)
        return (NULL);

    ft_strlcpy(full_path, base, base_len + 1); // Copie `base` dans `full_path`
    ft_strlcat(full_path, "/", base_len + ft_strlen(input) + 2); // Ajoute '/'
    ft_strlcat(full_path, input, base_len + ft_strlen(input) + 2); // Ajoute `input`

    return (full_path);
}

static t_dir_stack *g_dir_stack = NULL; // Pile globale

void push_dir(const char *dir)
{
    t_dir_stack *new_node;

    new_node = malloc(sizeof(t_dir_stack));
    if (!new_node)
        return;
    new_node->dir = strdup(dir);
    new_node->next = g_dir_stack;
    g_dir_stack = new_node;
}
char *pop_dir(void)
{
    t_dir_stack *top;
    char        *dir;

    if (!g_dir_stack)
        return (NULL); // Pile vide
    top = g_dir_stack;
    dir = top->dir;
    g_dir_stack = g_dir_stack->next;
    free(top);
    return (dir);
}

/**
 * ft_cd - Implémente la commande cd.
 * @argv: Tableau d'arguments (argv[0] = "cd").
 *
 * Retourne 0 en cas de succès, 1 en cas d'erreur.
 */

int ft_cd(char **argv)
{
    char cwd[1024];
    char *path;
    char *previous_dir;

    if (!getcwd(cwd, sizeof(cwd))) // Récupère le répertoire courant
    {
        perror("cd");
        return (1);
    }

    if (!argv[1] || ft_strcmp(argv[1], "~") == 0) // "cd" ou "cd ~"
    {
        path = getenv("HOME");
        if (!path || !*path)
        {
            ft_printf("cd: HOME not set\n");
            return (1);
        }
    }
    else if (ft_strcmp(argv[1], "-") == 0) // "cd -"
    {
        previous_dir = pop_dir();
        if (!previous_dir)
        {
            ft_printf("cd: No previous directory\n");
            return (1);
        }
        ft_printf("%s\n", previous_dir); // Affiche le répertoire précédent
        path = previous_dir;
    }
    else
        path = argv[1]; // Sinon, utilise le chemin donné

    if (chdir(path) == -1)
    {
        ft_printf("cd: %s: No such file or directory\n", path);
        return (1);
    }
    push_dir(cwd);    // Sauvegarde l'ancien répertoire
    update_pwd();     // Met à jour PWD et OLDPWD
    return (0);
}

