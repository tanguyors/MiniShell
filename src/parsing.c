
// parsing des commandes 
#include "../include/minishell.h"

/**
 * parse_command - Découpe la ligne en tokens (arguments).
 * @input: La ligne brute entrée par l'utilisateur.
 *
 * Retourne un tableau de chaînes de caractères (les arguments de la commande).
 */

char **parse_tokens(char *input) 
{
    char **tokens;
    
    if (!input || *input == '\0') 
    {
        return (NULL);
    }
    // Découpe la ligne avec ft_split, en séparant par les espaces
    tokens = ft_split(input, ' ');
    // Vérifie si la commande a été correctement découpée
    if (!tokens) 
    {
        perror("Parsing error"); // Affiche une erreur si la découpe échoue
        exit(EXIT_FAILURE);
    }
    return (tokens); // Retourne le tableau de tokens
}


static void p_redirection(int *i, char *str, struct s_shell **value)
{
	int j;

	if (is_redirect(str[(*i)])) 
	{
		printf("redirection:\n");
		insert_head(value, NULL);
		(*value)->token = TOKEN_WORD;
		(*value)->data[0] = str[(*i)++];
		(*value)->data[1] = '\0';
		if (is_redirect(str[(*i)])) 
		{
			(*value)->data[1] = str[(*i)++];
			(*value)->data[2] = '\0';
		}
		if (str[++(*i)] && is_alnum(str[(*i)]))
		{
			printf("infile:\n");
			insert_head(value, NULL);
			(*value)->token = TOKEN_INFILE;
			j = 0;
			while (str[(*i)] != '\0' && !is_space(str[(*i)])) 
				(*value)->data[j++] = str[(*i)++];
			(*value)->data[j] = '\0';
		}
		else
			perror("syntax error near unexpected token `newline'\n");
		(*i)++;
	}
}

/* appel récursif viable ? 
	incrémentation de i non pris en compte par p_command() 
	passer i a p_pipe par référence ? */
static void p_pipe(int i, char *str, struct s_shell **value)
{
	if (str[i] == '|')
	{
		printf("pipe:\n");
		insert_head(value, NULL);
		(*value)->token = TOKEN_PIPE;
		(*value)->data[0] = '|';
		(*value)->data[1] = '\0';
		i++;
		p_command(&i, str, value);
	}
}

void p_command(int *i, char *str, struct s_shell **value)
{
	int j;

	if (str[(*i)] && is_alnum(str[(*i)]))
	{
		printf("command:\n");
		insert_head(value, NULL);
		(*value)->token = TOKEN_CMD;
		j = 0;
		while (str[(*i)] != '\0' && !is_space(str[(*i)])) 
			(*value)->data[j++] = str[(*i)++];
		(*value)->data[j] = '\0';
		(*i)++;
		p_pipe((*i), str, value);
	}
}

/* uniquement considéré comme arg si str[i] = '-' */
static void p_arg(int *i, char *str, struct s_shell **value)
{
	int j;

	if (str[(*i)] && is_alpha(str[++(*i)]))
	{
		(*i)--;
		printf("arg:\n");
		insert_head(value, NULL);
		(*value)->token = TOKEN_ARG;
		j = 0;
		while (str[(*i)] != '\0' && !is_space(str[(*i)])) 
			(*value)->data[j++] = str[(*i)++];
		(*value)->data[j] = '\0';
		(*i)++;
		p_pipe((*i), str, value);
	}
}


struct s_shell *parsing(char *str, struct s_shell *value) 
{
    int i;

    i = 0;
    while (str[i] != '\0') 
    {
        while (is_space(str[i]))
            i++;
        p_redirection(&i, str, &value);
		p_command(&i, str, &value);
		p_arg(&i, str, &value);
		break;
    }
    return (value);
}


void parse_commands(char **tokens)
{
    t_builtin builtins[] = {
        {"pwd", ft_pwd},
        {"echo", ft_echo},
        {"env", ft_env},
        {"export", ft_export},
        {"unset", ft_unset},
        {"cd", ft_cd},
        {NULL, NULL}
    };
    int i = 0;

    if (!tokens[0]) // Commande vide
    {
        ft_printf("Error: Empty command\n");
        return;
    }

    // Parcourt la table des commandes internes
    while (builtins[i].name != NULL)
    {
        if (ft_strcmp(tokens[0], builtins[i].name) == 0)
        {
            builtins[i].func(tokens); // Appelle la fonction correspondante
            return;
        }
        i++;
    }

    // Si aucune commande builtin ne correspond
    ft_printf("minishell: %s: command not found\n", tokens[0]);
}
