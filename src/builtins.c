
//Implementations des commandes interne
#include "../include/minishell.h"

int ft_exit(char **argv, struct s_shell *shell)
{
    int argc;

    argc = 0;
    while (argv[argc])
        argc++;
    if (argc > 1)
    {
        ft_putstr_fd("too many arguments", 2);
        exit(1);
    }
    if (argv[0])
    {
        if (!is_str(argv[0]))
        {
            ft_putstr_fd("numeric argument required", 2);
            exit(2);
        }
    }
    free(shell->rl_input);
    if (argv[0])
        exit(ft_atoi(argv[0]));
	exit(shell->exit_code);
}

//------------------------------------------------------------------------------------------ECHO----------------------------------------------------------------
/**
 * ft_echo - Implémente le built-in echo.
 * @argv: Tableau d'arguments, argv[0] = "echo".
 *
 * Retourne 0 en cas de succès.
 */

/* Parse et récupère la variable d'environnement */
char *expand_variable(const char *var)
{
    size_t len = 0;
    char *var_name;
    char *value;

    while (var[len] && (ft_isalnum(var[len]) || var[len] == '_'))
        len++;
    var_name = ft_substr(var, 0, len);
    value = getenv(var_name);
    free(var_name);
    if (value)
        return (value);
    return ("");
}

int ft_echo(char **argv, struct s_shell *shell)
{
    int i = 0;
    int newline = 1;

    // Gère l'option "-n"
    while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }

    // Parcourt les arguments
    while (argv[i])
    {   
        if (argv[i][0] == '$') // Vérifie si c'est une variable d'environnement
        {
            // Récupère et affiche la valeur de la variable (sans le '$')
            ft_putstr_fd(expand_variable(argv[i] + 1), 1);
            if(argv[i][1] == '?')
            {
                ft_printf("%d", shell->exit_code);
            }
        }
        else
        {
            ft_putstr_fd(argv[i], 1); // Affiche l'argument normal
        }
        if (argv[i + 1])
            ft_putchar_fd(' ', 1); // Ajoute un espace entre les arguments
        i++;
    }
    shell->exit_code = 0;
    if (newline)
        ft_putchar_fd('\n', 1); // Ajoute un saut de ligne si nécessaire
    return (0);
}
 //--------------------------------------------------------------------------------------PWD------------------------------------------------------------------------
int ft_pwd(char **argv, struct s_shell *shell)
{
	(void)argv;
    char cwd[1024]; // Buffer pour stocker le chemin

    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        ft_printf("%s\n", cwd); // Affiche le répertoire courant avec un saut de ligne
        shell->exit_code = 0;
        return (0);
    } 
    else 
    {
        perror("pwd"); // Affiche une erreur si getcwd échoue
        shell->exit_code = 22;
    }
}
extern char **environ; 
//-----------------------------------------------------------------------------------ENV---------------------------------------------------------------------------------
/**
 * ft_env - Affiche toutes les variables d'environnement.
 * @argv: Les arguments passés à la commande (argv[0] = "env").
 *
 * Retourne 0 en cas de succès, 1 si des arguments sont passés.
 */
int ft_env(char **argv, struct s_shell *shell)
{
    int i = 0;

    // Vérifie si des arguments supplémentaires sont donnés
    if (argv[1])
    {
        ft_putstr_fd(" No such file or directory\n", 2);
        shell->exit_code = 1; // Met à jour le code de sortie
        return (1); // Indique une erreur
    }

    // Parcourt la variable globale environ et affiche chaque variable
    while (environ[i])
    {
        ft_printf("%s\n", environ[i]);
        i++;
    }
    shell->exit_code = 0;
    return (0); // Toujours succès
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
int ft_export(char **argv, struct s_shell *shell)
{
    int i = 0;

    // Si aucun argument n'est donné, affiche toutes les variables exportées
    if (!argv[0])
    {
        int j = 0;
        while (environ[j])
        {
            ft_printf("declare -x %s\n", environ[j]);
            j++;
        }
        return (0); // Toujours succès
    }

    // Parcourt les arguments donnés pour les ajouter/modifier
    while (argv[i])
    {
        // Vérifie si l'argument est un identifiant valide
        if (!is_valid_identifier(argv[i]))
        {
            //ft_printf("export: `%s': not a valid identifier\n", argv[i]);
            ft_putstr_fd(" not a valid identifier", 2);
            shell->exit_code = 1; // Met à jour le code de sortie
            return (1); // Indique une erreur
        }
        else
        {
            // Copie l'argument pour pouvoir le manipuler
            char *name = ft_strdup(argv[i]);
            // Cherche la position de '=' dans la chaîne
            char *value = ft_strchr(name, '=');
            if (value) // Si '=' est trouvé
            {
                *value = '\0'; // Coupe la chaîne en deux à '='
                value++; // Pointe sur la valeur après '='
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
    shell->exit_code = 0;
    return (0); // Toujours succès
}

//--------------------------------------------------------------------------------------------------------------------------UNSET---------------------------------------------------------------------------------------------
// Version definitive a revoir pour mettre a la norme !!!!!
int ft_unset(char **argv, struct s_shell *shell)
{
    int i = 0;

    if (!argv[0]) // Aucun argument : rien à faire
        return (0);

    while (argv[i])
    {
        if (!is_valid_identifier(argv[i]))
        {
            ft_putstr_fd(" not a valid identifier\n", 2);
            shell->exit_code = 1; // Met à jour le code de sortie
            return (1); // Indique une erreur
        }
        else
        {
            int j = 0;
            while (environ[j])
            {
                // Compare uniquement la partie avant le '='
                if (ft_strncmp(environ[j], argv[i], ft_strlen(argv[i])) == 0 &&
                    environ[j][ft_strlen(argv[i])] == '=')
                {
                    // Décale toutes les entrées après j
                    int k = j;
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
    shell->exit_code = 0;
    return (0); // Toujours succès
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

int ft_cd(char **argv, struct s_shell *shell)
{
    char cwd[1024];
    char *path;
    char *previous_dir;

    if (!getcwd(cwd, sizeof(cwd))) // Récupère le répertoire courant
    {
        perror("cd");
        shell->exit_code = 1; // Met à jour le code de sortie
        return (1); // Indique une erreur
    }

    if (!argv[1] || ft_strcmp(argv[1], "~") == 0) // "cd" ou "cd ~"
    {
        path = getenv("HOME");
        if (!path || !*path)
        {
            ft_putstr_fd(" HOME not set\n", 2);
            shell->exit_code = 1; // Met à jour le code de sortie
            return (1); // Indique une erreur
        }
    }
    else if (ft_strcmp(argv[1], "-") == 0) // "cd -"
    {
        previous_dir = pop_dir();
        if (!previous_dir)
        {
            ft_putstr_fd(" No previous directory\n", 2);
            shell->exit_code = 1; // Met à jour le code de sortie
            return (1); // Indique une erreur
        }
        ft_printf("%s\n", previous_dir); // Affiche le répertoire précédent
        path = previous_dir;
    }
    else
        path = argv[1]; // Sinon, utilise le chemin donné

    if (chdir(path) == -1)
    {
        ft_putstr_fd(" No such file or directory\n", 2);
        shell->exit_code = 1; // Met à jour le code de sortie
        return (1); // Indique une erreur
    }

    push_dir(cwd); // Sauvegarde l'ancien répertoire
    update_pwd(); // Met à jour PWD et OLDPWD
    shell->exit_code = 0;
    return (0); // Toujours succès
}