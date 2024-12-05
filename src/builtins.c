
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

/**
 * update_pwd - Met à jour les variables d'environnement PWD et OLDPWD.
 */
static void update_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)))
    {
        setenv("OLDPWD", getenv("PWD"), 1); // Met à jour OLDPWD avec l'ancien PWD
        setenv("PWD", cwd, 1);              // Met à jour PWD avec le chemin actuel
    }
}

typedef struct s_dir_stack
{
    char                *dir; // Répertoire sauvegardé
    struct s_dir_stack  *next;
}   t_dir_stack;


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
    // Récupère le répertoire courant
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("cd");
        return (1);
    }
    if (!argv[1]) // Aucun argument : aller dans $HOME
    {
        path = getenv("HOME");
        if (!path || *path == '\0')
        {
            ft_printf("cd: HOME not set\n");
            return (1);
        }
    }
    else if (ft_strcmp(argv[1], "-") == 0) // "cd -" : revenir au dernier répertoire
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
    {
        path = argv[1]; // Chemin spécifié par l'utilisateur
    }

    // Vérifie si le chemin existe
    if (access(path, F_OK) == -1)
    {
        ft_printf("cd: %s: No such file or directory\n", path);
        return (1);
    }
    if (access(path, X_OK) == -1)
    {
        ft_printf("cd: %s: Permission denied\n", path);
        return (1);
    }
    // Ajoute le répertoire courant à la pile
    push_dir(cwd);
    // Change le répertoire
    if (chdir(path) == -1)
    {
        perror("cd");
        return (1);
    }
    // Met à jour PWD après changement
    if (getcwd(cwd, sizeof(cwd)))
        setenv("PWD", cwd, 1);

    return (0);
}


