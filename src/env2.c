#include "../include/minishell.h"

/*
** export_display: Affiche toutes les variables exportées.
*/
static int	export_display(void)
{
	extern char **environ;
	int	j;

	j = 0;
	while (environ[j])
	{
		ft_printf("declare -x %s\n", environ[j]);
		j++;
	}
	return (0);
}

/*
** export_update: Met à jour ou ajoute une variable d'environnement.
** Elle découpe l'argument "KEY=VALUE" en deux parties,
** puis appelle setenv avec le flag 1 (pour écraser).
*/
static void	export_update(char *arg)
{
	char	**split;
	char	*name;
	char	*value;

	split = ft_split(arg, '=');
	if (!split)
		return ;
	name = ft_strdup(split[0]);
	if (split[1])
		value = ft_strdup(split[1]);
	else
		value = ft_strdup("");
	setenv(name, value, 1);
	free(name);
	free(value);
	free(split[0]);
	if (split[1])
		free(split[1]);
	free(split);
}

/*
** ft_export: Gère la commande export.
** Si aucun argument n'est donné, affiche l'environnement.
** Sinon, pour chaque argument, vérifie la validité et met à jour l'environnement.
*/
int	ft_export(char **argv, struct s_shell *shell, struct s_shell *head)
{
	int	i;

	i = 1;
	if (!argv[1])
		return (export_display());
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_putstr_fd(" not a valid identifier\n", 2);
			shell->exit_code = 1;
			return (1);
		}
		export_update(argv[i]);
		i++;
	}
	shell->exit_code = 0;
	return (0);
}