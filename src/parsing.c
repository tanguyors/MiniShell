
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


/* Incomplete, Parse the input to call the correct function,
    and execute the right command */
void parse_commands(char **tokens) 
{
    if (ft_strcmp(tokens[0], "pwd") == 0)
    {
        ft_pwd();
    }
    if (ft_strcmp(tokens[0], "echo") == 0)
    {
        ft_echo(tokens);
    }
    if (ft_strcmp(tokens[0], "env") == 0)
    {
        ft_env(tokens);
    }
    if (ft_strcmp(tokens[0], "export") == 0)
    {
        ft_export(tokens);
    }
    // ...
}
