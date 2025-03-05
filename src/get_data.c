/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:48:42 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 18:15:00 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Remise a la norme de l'initialisation de ta structure t_builtin */
void initialize_builtin(t_builtin *builtin)
{
    builtin[0] = (t_builtin){"pwd", ft_pwd};
    builtin[1] = (t_builtin){"echo", ft_echo};
    builtin[2] = (t_builtin){"env", ft_env};
    builtin[3] = (t_builtin){"export", ft_export};
    builtin[4] = (t_builtin){"unset", ft_unset};
    builtin[5] = (t_builtin){"cd", ft_cd};
    builtin[6] = (t_builtin){"exit", ft_exit};
	builtin[7] = (t_builtin){NULL, NULL};
}

/* implémenter strtok, strcpy et strcat */
char *get_absolute_path(char *command)
{
    static char path[1024];
	char path_copy[1024];
    char *path_env;
    char *dir;

	path_env = getenv("PATH");
    if (path_env == NULL) 
	{
		ft_putstr_fd("Error PATH not found\n", 2);
        return (NULL);
    }
    ft_strncpy(path_copy, path_env, sizeof(path_copy));
    path_copy[sizeof(path_copy) - 1] = '\0';
    dir = ft_strtok(path_copy, ":");
    while (dir != NULL) 
	{
        ft_strcpy(path, dir);
        ft_strcat(path, "/");
        ft_strcat(path, command);
        if (access(path, X_OK) == 0) 
            return (path);
        dir = ft_strtok(NULL, ":");
    }
    return (NULL);
}

char **get_arg_data(struct s_shell *current, struct s_shell *shell)
{
    char **data;
    int i;

	i = 0;
	data = ft_calloc(1024, sizeof(char *));
	if (!data)
		exit_with_error("allocation error", NULL, 1);
    while (current && current->token != TOKEN_PIPE)
    {
        if (current->token == TOKEN_SIMPLE_QUOTE)
        {
            data[i] = expand_token(current->data, 1, shell);
        }
        else
        {
            data[i] = expand_token(current->data, 0, shell);
        }
        i++;
        current = current->next;
    }
    data[i] = NULL;
    return (data);
}

/* Data des commandes + leurs arguments 
	mémoire alloué au char ** */
char **get_all_data(struct s_shell *current)
{
    int i;
	struct s_shell *p_arg;
	char **data;

	data = ft_calloc(1024, sizeof(char *));
	if (!data)
		exit_with_error("allocation error", NULL, 1);
	i = 0;
	p_arg = current;
	while (p_arg)
	{
		if (p_arg->token == TOKEN_CMD || p_arg->token == TOKEN_ARG)
		{
			data[i] = p_arg->data;
			i++;
		}
		if (p_arg->token == TOKEN_PIPE)
			break;
		p_arg = p_arg->next;
	}
	return (data);
}

/* Version adapté de ta fonction parse_command() pour la liste chaînée 
	Lis la commande étant dans le token_cmd et la compare avec le tableau builtin 
	si la commande correspond celle ci est ensuite executé
	Pour chaque argument dans token_arg ils sont stocké dans data qui est ensuite,
	donné comme char ** aux fonctions builtin */
/* cmd_execution() passe des tests classiques mais il y aura certainement des changement a faire par la suite
	dans celle ci mais aussi les builtin */
void extract_data(struct s_shell *shell, struct s_shell *current)
{
	char **data;

	data = get_arg_data(current, shell);
	cmd_execution(shell, current, data);
	free_array(data);
}
