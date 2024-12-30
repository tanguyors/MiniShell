
// gestion de l'executions des commande
#include "../include/minishell.h"

/* Remise a la norme de l'initialisation de ta structure t_builtin */
static void initialize_builtin(t_builtin *builtin) 
{
    builtin[0] = (t_builtin){"pwd", ft_pwd};
    builtin[1] = (t_builtin){"echo", ft_echo};
    builtin[2] = (t_builtin){"env", ft_env};
    builtin[3] = (t_builtin){"export", ft_export};
    builtin[4] = (t_builtin){"unset", ft_unset};
    builtin[5] = (t_builtin){"cd", ft_cd};
    builtin[6] = (t_builtin){NULL, NULL};
}
/* Version adapté de ta fonction parse_command() pour la liste chaînée 
	Lis la commande étant dans le token_cmd et la compare avec le tableau builtin 
	si la commande correspond celle ci est ensuite executé
	Pour chaque argument dans token_arg ils sont stocké dans data qui est ensuite,
	donné comme char ** aux fonctions builtin */

/* J'ai adapté l'éxecution de telle sorte a ne pas modifié tes fonctions builtin
	si ça te parrais mieux de les modifier tu peux faire comme ça et changer également cette fonction */

/* cmd_execution() passe des tests classiques mais il y aura certainement des changement a faire par la suite
	dans celle ci mais aussi les builtin */
void cmd_execution(struct s_shell *current)
{
	t_builtin builtin[7];
    int i;
	struct s_shell *p_arg;
	char *data[1024];

	initialize_builtin(builtin);
	i = 0;
	p_arg = current;
	//printf("p_arg: %s\n", p_arg->data);
	while (p_arg)
	{
		if (p_arg->token == TOKEN_ARG)
		{
			data[i] = p_arg->data;
			//printf("data[%d]: %s\n", i, data[i]);
			i++;
		}
		if (p_arg->token == TOKEN_PIPE)
			break;
		p_arg = p_arg->next;
	}
	i = 0;
    // Parcourt la table des commandes internes
    while (builtin[i].name != NULL)
    {
        if (ft_strcmp(current->data, builtin[i].name) == 0)
        {
            builtin[i].func(data); // Appelle la fonction correspondante.
            return;
        }
        i++;
    }
	//free_array(data);
    // Si aucune commande builtin ne correspond
    ft_printf("minishell: %s: command not found\n", current->data);
}

static void redir_input(struct s_shell *current)
{
	if (current && current->next)
		current = current->next;
	if (current->token == TOKEN_FILE)
	{
		/* execution de la redirection avec le TOKEN_FILE */
	}
	
}

static void redir_output(struct s_shell *current)
{
	if (current && current->next)
		current = current->next;
	if (current->token == TOKEN_FILE)
	{
		/* execution de la redirection avec le TOKEN_FILE */
	}
}

static void redir_append(struct s_shell *current)
{
	if (current && current->next)
		current = current->next;
	if (current->token == TOKEN_FILE)
	{
		/* execution de la redirection avec le TOKEN_FILE */
	}
}

static void redir_heredoc(struct s_shell *current)
{
	if (current && current->next)
		current = current->next;
	if (current->token == TOKEN_FILE)
	{
		/* execution de la redirection avec le TOKEN_FILE */
	}
}

/* Redirige vers le bon token de redirection */
void redirection_execution(struct s_shell *current)
{
	if (current->token == REDIR_INPUT)
	{
		redir_input(current);
	}
	else if (current->token == REDIR_OUTPUT)
	{
		redir_output(current);
	}
	else if (current->token == REDIR_APPEND)
	{
		redir_append(current);
	}
	else if (current->token == 	REDIR_HEREDOC)
	{
		redir_heredoc(current);
	}
}

/* Permet de trier les executions des commandes,
	en parcourant les tokens de la liste chaînée */
/* Les pipes sont encore a implémenter, et le passage par référence,
	de current aux fonctions d'éxécution sera peut être necessaire */
void parse_execution(struct s_shell *head)
{
	struct s_shell *current;

	current = head;
	while (current)
	{
		if (current->token)
		{
			if (current->token == TOKEN_CMD) // relié a TOKEN_ARG, cherche un token ARG
			{
				cmd_execution(current);
			}
			else if (current->token == is_token_red(current->token)) // relié a TOKEN_RED, cherche REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND, REDIR_HEREDOC
			{
				redirection_execution(current);
			}
			else if (current->token == TOKEN_PIPE) // rappel a la fonction lié a TOKEN_CMD	
			{
				current = current->next;
				cmd_execution(current);
			}
			
		}
		current = current->next;
	}
}