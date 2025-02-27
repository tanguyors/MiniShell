
//Implementations des commandes interne
#include "../include/minishell.h"



int ft_exit(char **argv, struct s_shell *shell, struct s_shell *head)
{
    int argc;
    int value;

    value = 0;
    argc = 0;
    while (argv[argc])
        argc++;
    free(shell->rl_input);
    rl_clear_history();
    free_list(head);
    if (argc > 2)
    {
        ft_putstr_fd("too many arguments", 2);
        free_array(argv);
        exit(1);
    }
    if (argv[1])
    {
        if (!is_str(argv[0]))
        {
            ft_putstr_fd("numeric argument required", 2);
            free_array(argv);
            exit(2);
        }
    }
    if (argv[0])
    {
        value = ft_atoi(argv[0]);
        free_array(argv);
        exit(value);
    }
	exit(shell->exit_code);
}

//------------------------------------------------------------------------------------------ECHO----------------------------------------------------------------
/**
 * expand_token - Parcourt `input` et remplace chaque occurrence de '$...' par
 *                la valeur de la variable d'environnement correspondante.
 * @input:             La chaîne d'entrée à analyser.
 * @is_in_single_quote: Indique si ce token était entouré de single quotes (1 = oui, 0 = non).
 * @shell:             Pointeur vers la structure s_shell (pour récupérer shell->exit_code, etc.).
 *
 * Retourne un pointeur vers la nouvelle chaîne allouée, ou NULL en cas d'erreur.
 * À toi de free la chaîne après usage pour éviter les leaks.
 *
 * Cette fonction ne fait pas de réallocation dynamique
 *        si la variable expandée est très longue. Pour le minishell,
 *        tu peux prévoir un buffer plus grand ou ajouter une logique
 *        de réallocation
 */

char *expand_token(const char *input, int is_in_single_quote, struct s_shell *shell)
{
    // 1) Sécurité : si input est NULL, on renvoie NULL directement.
    if (!input)
        return NULL;

    // 2) Si c'est entouré de single quotes, on n’expanse rien
    //    => on retourne juste une copie brute de la chaîne.
    if (is_in_single_quote)
        return ft_strdup(input);
    // 3) Déterminer la taille d'allocation.
    //    Pour être sûr de ne pas manquer de place en cas de variables plus longues,
    //    on peut faire un "x2" ou "x3" sur la taille d’input. 
    //    À adapter selon tes besoins.
    size_t input_len = ft_strlen(input);
    size_t max_len   = input_len * 2 + 1;  // +1 pour le \0 final

    char *result = malloc(max_len);
    if (!result)
        return NULL; // Erreur d'allocation mémoire

    size_t i = 0; // Index pour parcourir `input`
    size_t j = 0; // Index pour remplir `result`

    while (input[i])
    {
        // Détection du caractère '$'
        if (input[i] == '$')
        {
            // Cas particulier : '$?' => code de retour de la dernière commande
            if (input[i + 1] == '?')
            {
                // Convertir shell->exit_code en string via ft_itoa
                char *code_str = ft_itoa(shell->exit_code);
                if (code_str)
                {
                    // Recopier code_str dans result
                    size_t k = 0;
                    while (code_str[k] && (j + 1) < max_len)
                        result[j++] = code_str[k++];
                    free(code_str);
                }
                // On a consommé 2 caractères : '$?'
                i += 2;
            }
            else
            {
                // Avancer pour ignorer le '$'
                i++;

                // Lire le nom de variable (alphanumérique + underscore)
                size_t var_start = i;
                while ((input[i] >= '0' && input[i] <= '9') ||
                       (input[i] >= 'A' && input[i] <= 'Z') ||
                       (input[i] >= 'a' && input[i] <= 'z') ||
                        input[i] == '_')
                {
                    i++;
                }
                // var_len = longueur du nom de variable
                size_t var_len = i - var_start;

                // Extraire le nom dans une nouvelle chaîne
                char *var_name = ft_substr(input, var_start, var_len);
                if (!var_name)
                {
                    free(result);
                    return NULL; // Erreur d'allocation
                }

                // Récupérer la valeur (via getenv ou ta gestion d'environ)
                char *var_value = getenv(var_name); 
                free(var_name);

                // Si la variable n’existe pas, on considère que c’est vide
                if (!var_value)
                    var_value = "";

                // Recopier var_value dans result
                size_t k = 0;
                while (var_value[k] && (j + 1) < max_len)
                    result[j++] = var_value[k++];
            }
        }
        else
        {
            // Caractère normal => on le recopie dans result
            if ((j + 1) < max_len)
                result[j++] = input[i];
            i++;
        }
    }
    // Terminaison de la chaîne
    result[j] = '\0';

    return result;
}
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

/*int ft_echo(char **argv, struct s_shell *shell)
{
    int i = 0;
    int newline = 1;

    // 1) Gérer l'option '-n'
    while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }
    // 2) Parcourir les arguments restants
    while (argv[i])
    {
        // Ici, on suppose qu'on ne gère pas la différence single/double quotes
        // dans echo, donc on met 'is_in_single_quote = 0' pour autoriser l'expansion.
        // (Si ton parsing stocke un flag, tu le passes en paramètre.)
        char *expanded = expand_token(argv[i], 0, shell);

        ft_putstr_fd(expanded, 1);

        // Afficher un espace entre les arguments (sauf avant le \n final)
        if (argv[i + 1])
            ft_putchar_fd(' ', 1);

        // Penser à free la nouvelle chaîne pour éviter une fuite mémoire
        free(expanded);

        i++;
    }

    // 3) Ajouter le \n si on n’a pas rencontré de '-n'
    if (newline)
        ft_putchar_fd('\n', 1);

    // 4) Ajuster le code de sortie si besoin
    shell->exit_code = 0;

    return (0);
}*/
int ft_echo(char **argv, struct s_shell *shell, struct s_shell *head)
{
    int i = 1;
    int newline = 1;
    char *result;

    /*if (argv[i] && argv[i + 1])
    {
        while (argv[i])
        {
            result = ft_strjoin(argv[i], argv[i + 1]);
            i++;
        }
    }
    if (result)
        printf("test result: %s\n", result);*/
    // Gérer l'option -n
    i = 1;
    while (argv[i] && ft_strcmp(argv[i], "-n") == 0)
    {
        newline = 0;
        i++;
    }

    // Afficher directement argv[i], déjà expansé
    while (argv[i])
    {
        ft_putstr_fd(argv[i], 1);
        //if (argv[i + 1])
            //ft_putchar_fd(' ', 1);
        i++;
    }

    if (newline)
        ft_putchar_fd('\n', 1);

    shell->exit_code = 0;
    return (0);
}

 //--------------------------------------------------------------------------------------PWD------------------------------------------------------------------------
int ft_pwd(char **argv, struct s_shell *shell, struct s_shell *head)
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

//-----------------------------------------------------------------------------------ENV---------------------------------------------------------------------------------
/**
 * ft_env - Affiche toutes les variables d'environnement.
 * @argv: Les arguments passés à la commande (argv[0] = "env").
 *
 * Retourne 0 en cas de succès, 1 si des arguments sont passés.
 */
int ft_env(char **argv, struct s_shell *shell, struct s_shell *head)
{
    int i = 0;
    extern char **environ; 
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
int ft_export(char **argv, struct s_shell *shell, struct s_shell *head)
{
    int i = 1;
    extern char **environ; 
    // Si aucun argument n'est donné, affiche toutes les variables exportées
    if (!argv[1])
    {
        int j = 0;
        while (environ[j])
        {
            ft_printf("declare -x %s\n", environ[j]);
            j++;
        }
        return (0); // Toujours succès
    }
    if (argv[1])
        ft_strcat(argv[0], argv[1]);
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
int ft_unset(char **argv, struct s_shell *shell, struct s_shell *head)
{
    int i = 1;
    extern char **environ; 

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

int ft_cd(char **argv, struct s_shell *shell, struct s_shell *head)
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