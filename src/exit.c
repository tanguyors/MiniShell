#include "../include/minishell.h"

/*
** check_exit_args:
**   - Compte le nombre d'arguments passés à ft_exit.
**   - Si plus de 2 arguments sont fournis, affiche un message d'erreur.
**   - Si un argument est fourni mais n'est pas numérique, affiche un message.
** Retourne 0 en cas de succès, 1 si trop d'arguments et 2 si l'argument n'est pas numérique.
*/
static int	check_exit_args(char **argv)
{
	int	argc;

	argc = 0;
	while (argv[argc])
		argc++;
	if (argc > 2)
	{
		ft_putstr_fd("too many arguments\n", 2);
		return (1);
	}
	if (argc == 2 && !is_str(argv[1]))
	{
		ft_putstr_fd("numeric argument required\n", 2);
		return (2);
	}
	return (0);
}

/*
** ft_exit:
**   - Libère les ressources (rl_input, historique, liste chainée, argv).
**   - Vérifie les arguments via check_exit_args().
**   - Quitte le programme avec le code de sortie approprié.
*/
int	ft_exit(char **argv, struct s_shell *shell, struct s_shell *head)
{
	int	ret;
	int	exit_val;

	ret = check_exit_args(argv);
	free(shell->rl_input);
	rl_clear_history();
	free_list(head);
	free_array(argv);
	if (ret)
		exit(ret);
	/* Si un argument est passé (argv[1]), on l'interprète comme le code de sortie.
	   Sinon, on utilise shell->exit_code. */
	if (argv[1])
		exit_val = ft_atoi(argv[1]);
	else
		exit_val = shell->exit_code;
	exit(exit_val);
}