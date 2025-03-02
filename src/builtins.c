
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
    //free_stack(g_dir_stack);
    update_pwd(); // Met à jour PWD et OLDPWD
    shell->exit_code = 0;
    return (0); // Toujours succès
}
