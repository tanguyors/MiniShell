
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

/* Fonction complémentaire de p_redirection elle permet de traiter
	les fichier in file et out file et de les tokenizer */
static void r_in_out_file(int *i, char *str, struct s_shell **head, int *stop_flag)
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
	{
		perror("syntax error near unexpected token `newline'\n");
		*stop_flag = 1;
	}
	while (is_space(str[(*i)]))
		(*i)++;
}

/* Fonction permettant de déterminer le type de redirection */
enum e_tokens which_red(int *i, char *str)
{
	printf("token : %c\n", str[(*i)]);
    if (str[(*i)] == '<' && str[(*i) + 1] == '<') 
        return (REDIR_HEREDOC);
    else if (str[(*i)] == '>' && str[(*i) + 1] == '>') 
        return (REDIR_APPEND);
    else if (str[(*i)] == '<') 
        return (REDIR_INPUT);
    else if (str[(*i)] == '>') 
        return (REDIR_OUTPUT);
    return (TOKEN_RED);
}

/* Fonction permettant de tokenizer les redirections
	celles ci pouvant être n'importe ou dans la string */
static void p_redirection(int *i, char *str, struct s_shell **head, int *stop_flag)
{
	struct s_shell *tail;

	if (is_redirect(str[(*i)])) 
	{
		printf("redirection:\n");
		insert_tail(head, NULL, "TOKEN_RED");
		tail = get_last_node(*head);
		tail->token = which_red(i, str);
		tail->data[0] = str[(*i)++];
		tail->data[1] = '\0';
		if (is_redirect(str[(*i)])) 
		{
			tail->data[1] = str[(*i)++];
			tail->data[2] = '\0';
		}
		r_in_out_file(i, str, head, stop_flag);
	}
}

/* Fonction permettant de tokenizer les pipes */
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
/* Fonction permettant de tokenizer les commandes */
int p_command(int *i, char *str, struct s_shell **head, int *stop_flag)
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
		while (str[(*i)] != '\0' && !is_space(str[(*i)]) && !is_spec_char(str[(*i)]))
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

/* Fonction permettant de tokenizer les arguments de commandes */
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

/* Fonction complémentaire a p_quotes, permet de vérifier s'il y a des double quotes
	et tokenize en conséquence */
static void p_double_quotes(int *i, char *str, struct s_shell *tail)
{
	int j;

	j = 0;
	if (str[(*i)] == '"')
	{
		(*i)++;
		tail->token = TOKEN_DOUBLE_QUOTE;
		while (str[(*i)] && (str[(*i)] != '"'))
		{
			while(str[(*i)] == '"')
				(*i)++;
			
			tail->data[j++] = str[(*i)];
			(*i)++;
		}
		tail->data[j] = '\0';
		(*i)++;
	}
}

/* Fonction de vérification et tokenization des quotes */
static void p_quotes(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	j = 0;
	insert_tail(head, NULL, "TOKEN_QUOTES");
	tail = get_last_node(*head);
	tail->token = TOKEN_CMD;
	if (tail->token)
		printf("tail exist, token: %d\n", tail->token);
	if (str[(*i)] == 39)
	{
		(*i)++;
		tail->token = TOKEN_SIMPLE_QUOTE;
		while (str[(*i)] && (str[(*i)] != 39))
		{
			while(str[(*i)] == 39)
				(*i)++;
			tail->data[j++] = str[(*i)];
			(*i)++;
		}
		tail->data[j] = '\0';
		(*i)++;
	}
	p_double_quotes(i, str, tail);
}

struct s_shell *post_parsing_condition(struct s_shell *current, int *break_flag)
{
	if (current->token == TOKEN_CMD && current->next->token == TOKEN_CMD)
		current->next->token = TOKEN_ARG;
	if (current->token != TOKEN_PIPE && current->next->token == TOKEN_CMD)
		current->next->token = TOKEN_ARG;
	if (current->token == TOKEN_PIPE && current->next->token == TOKEN_PIPE 
	|| current->token == TOKEN_PIPE && current->next->token != TOKEN_CMD)
	{
		perror("bash: syntax error near unexpected token `|'");
		*break_flag = 1;
	}
	return (current);
}

/* Second parsing corrige la succession de 2 commandes,
	succession d'une prochaine commande sans pipe  */
struct s_shell *p_post_parsing(struct s_shell *head)
{
	struct s_shell *current;
	int break_flag;

	break_flag = 0;
	current = head;
	while (current)
	{
		if (current && current->next)
		{
			current = post_parsing_condition(current, &break_flag);
			if (break_flag)
				break;
		}
		if (current->token == TOKEN_DOUBLE_QUOTE || current->token == TOKEN_SIMPLE_QUOTE)
			current->token = TOKEN_ARG;
		if (current->token == TOKEN_PIPE && !current->next)
		{
			perror("bash: syntax error near unexpected token `|'");
			break;
		}
		current = current->next;
	}
	return (head);
}

struct s_shell *pre_parsing(char *str, struct s_shell *head, int *stop_flag) 
{
	int i;

    i = 0;
 	while (str[i] != '\0') 
    {
        while (is_space(str[i]))
            i++;
        if (is_redirect(str[i]))
			p_redirection(&i, str, &head, stop_flag);
        i++;
    }
	return (head);
}

void directory_parsing(int *i, char *str, int *stop_flag)
{
	if (str[(*i)] == '/')
	{
		printf("bash: %s: Is a directory\n", str);
		*stop_flag = 1;
	}
}

/* Parsing principale permettant de parcourir l'entièreté de la string
	et y crée une liste chainé avec les différentes valeurs et tokens */
struct s_shell *parsing(char *str, struct s_shell *head)
{
    int i;
	int stop_flag;

    i = 0;
	stop_flag = 0;
	//head = pre_parsing(str, head);
	directory_parsing(&i, str, &stop_flag);
    while (str[i] != '\0' && !stop_flag) 
    {
        while (is_space(str[i]))
            i++;
        if (is_redirect(str[i]))
			p_redirection(&i, str, &head, &stop_flag);
        if (!is_spec_char(str[i]))
            p_command(&i, str, &head, &stop_flag);
        else if (str[i] == '-')
            p_arg(&i, str, &head);
		else if (str[i] == 39 || str[i] == '"')
            p_quotes(&i, str, &head);
		else if (str[i] == '|')
            p_pipe(&i, str, &head);
        else
            i++;
    }
	head = p_post_parsing(head);
    return (head);
}
