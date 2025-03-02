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
