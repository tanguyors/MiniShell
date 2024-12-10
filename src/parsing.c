
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

static void r_in_out_file(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	while (is_space(str[(*i)]))
		(*i)++;
	if (str[(*i)] && is_alnum(str[(*i)]))
	{
		insert_tail(head, NULL, "TOKEN_FILE");
		tail = get_last_node(*head);
		tail->token = TOKEN_FILE;
		j = 0;
		while (str[(*i)] != '\0' && !is_space(str[(*i)])) 
			tail->data[j++] = str[(*i)++];
		tail->data[j] = '\0';
		printf("test :%s\n", tail->data);
	}
	else
		perror("syntax error near unexpected token `newline'\n");
	while (is_space(str[(*i)]))
		(*i)++;
}


static void p_redirection(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;

	if (is_redirect(str[(*i)])) 
	{
		printf("redirection:\n");
		insert_tail(head, NULL, "TOKEN_RED");
		tail = get_last_node(*head);
		tail->token = TOKEN_RED;
		tail->data[0] = str[(*i)++];
		tail->data[1] = '\0';
		if (is_redirect(str[(*i)])) 
		{
			tail->data[1] = str[(*i)++];
			tail->data[2] = '\0';
		}
		r_in_out_file(i, str, head);
	}
}

static int p_pipe(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;

	if (str[(*i)] == '|')
	{
		printf("pipe:\n");
		insert_tail(head, NULL, NULL);
		tail = get_last_node(*head);
		tail->token = TOKEN_PIPE;
		tail->data[0] = '|';
		tail->data[1] = '\0';
		(*i)++;
		while (is_space(str[(*i)]))
			(*i)++;
	}
	return (1);
}

/* implementer un deuxième parsing afin de résoudre TOKEN_CMD -> TOKEN_CMD */
int p_command(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	while (is_space(str[(*i)]))
		(*i)++;
	if (str[(*i)] && !is_spec_char(str[(*i)]) && !is_space(str[(*i)]))
	{
		printf("command:\n");
		insert_tail(head, NULL, NULL);
		tail = get_last_node(*head);
		tail->token = TOKEN_CMD;
		//while (is_space(str[(*i)]))
			//(*i)++;
		j = 0;
		while (str[(*i)] != '\0' && !is_space(str[(*i)]))
		{
			if (!is_space(str[(*i)]))
				tail->data[j++] = str[(*i)];
			(*i)++;
		}	
		tail->data[j] = '\0';
        //while (is_space(str[(*i)]))
            //(*i)++;
	}
	return (1);
}

static int p_arg(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	while (is_space(str[(*i)]))
		(*i)++;

	if (str[(*i)] && str[(*i)] == '-')
	{
		printf("arg:\n");
		insert_tail(head, NULL, NULL);
		tail = get_last_node(*head);
		tail->token = TOKEN_ARG;
		j = 0;
		tail->data[j++] = '-';
		(*i)++;
		while (str[(*i)] != '\0' && !is_spec_char(str[(*i)]))
		{
			if (!is_space(str[(*i)]))
				tail->data[j++] = str[(*i)];
			(*i)++;
		}	
		tail->data[j] = '\0';
	}
	return (1);
}

static void p_quotes(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	j = 0;
	insert_tail(head, NULL, "TOKEN_QUOTES");
	tail = get_last_node(*head);
	if (str[(*i)++] == 39)
	{
		tail->token = TOKEN_SIMPLE_QUOTE;
		while (str[(*i)++] != 39)
		{
			tail->data[j++] = str[(*i)];
		}
	}
	else if (str[(*i)++] != '"')
	{
		tail->token = TOKEN_DOUBLE_QUOTE;
		while (str[(*i)++] != '"')
		{
			tail->data[j++] = str[(*i)];
		}
	}
}

struct s_shell *parsing(char *str, struct s_shell *head) 
{
    int i;

    i = 0;
    while (str[i] != '\0') 
    {
        while (is_space(str[i]))
            i++;
        if (is_redirect(str[i]))
			p_redirection(&i, str, &head);
        else if (!is_spec_char(str[i]))
            p_command(&i, str, &head);
        else if (str[i] == '-')
            p_arg(&i, str, &head);
		else if (str[i] == '|')
            p_pipe(&i, str, &head);
		else if (str[i] == '~' || str[i] == '"')
            p_quotes(&i, str, &head);
        else
            i++;
    }
    return (head);
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
