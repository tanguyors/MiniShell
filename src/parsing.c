
// parsing des commandes 
#include "../include/minishell.h"

/* Fonction complémentaire de p_redirection elle permet de traiter
	les fichier in file et out file et de les tokenizer */
static void r_in_out_file(int *i, char *str, struct s_shell **head, int *stop_flag)
{
	struct s_shell *tail;
	int j;
	while (is_space(str[(*i)]))
		(*i)++;
	//printf("i = %d, char = %c\n", *i, str[*i]);
	if (str[(*i)] /* && is_alnum(str[(*i)]) */)
	{
		insert_tail(head, NULL, "TOKEN_FILE");
		tail = get_last_node(*head);
		tail->token = TOKEN_FILE;
		j = 0;
		while (str[(*i)] != '\0' && !is_redirect(str[(*i)]) && !is_space(str[(*i)]) && str[(*i)] != '|')
		{
			if (is_quotes(str[(*i)]))
				(*i)++;
			else
				tail->data[j++] = str[(*i)++];
		}
		tail->data[j] = '\0';
		//printf("i = %d, char = %c\n", *i, str[*i]);
	}
	else
	{
		//free(str);
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		*stop_flag = 1;
	}
	//ft_printf("test data: %s\n", tail->data);
	while (is_space(str[(*i)]))
		(*i)++;
	if (is_redirect(str[(*i)]))
		p_redirection(i, str, head, stop_flag);
	//printf("i = %d, char = %c\n", *i, str[*i]);
}

/* Fonction permettant de déterminer le type de redirection */
enum e_tokens which_red(int *i, char *str)
{
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
void p_redirection(int *i, char *str, struct s_shell **head, int *stop_flag)
{
	struct s_shell *tail;
	if (is_redirect(str[(*i)]))
	{
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

static void std_arg(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	insert_tail(head, NULL, NULL);
	tail = get_last_node(*head);
	tail->token = TOKEN_ARG;
	j = 0;
	while (str[(*i)] != '\0' && !is_spec_char_no_space(str[(*i)]))
	{
		if (is_space(str[(*i)]) && str[(*i) + 1 ] != '|')
			tail->data[j++] = ' ';
		while (str[(*i)] != '\0' && is_space(str[(*i)]))
			(*i)++;
		if (is_spec_char_no_space(str[(*i)]))
			break;
		while (str[(*i)] != '\0' && is_quotes(str[(*i)]))
			(*i)++;
		if (is_spec_char_no_space(str[(*i)]))
			break;
		tail->data[j++] = str[(*i)];
		(*i)++;
	}
	tail->data[j] = '\0';
}

/* Fonction permettant de tokenizer les arguments de commandes */
static int p_arg(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	if (str[(*i)])
	{
		while (is_space(str[(*i)]))
			(*i)++;
	}
	if (str[(*i)] && str[(*i)] == '-')
	{
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
	else if (str[(*i)] && !is_spec_char_no_space(str[(*i)]))
		std_arg(i, str, head);
	return (1);
}

/* Fonction complémentaire a p_quotes, permet de vérifier s'il y a des double quotes
	et tokenize en conséquence */
static void p_double_quotes(int *i, char *str, struct s_shell **head)
{
    int j = 0;

    if (str[*i] == '"')
    {
		// Insertion d'un nouveau nœud pour les double quotes
		insert_tail(head, NULL, "TOKEN_QUOTES");
		struct s_shell *tail = get_last_node(*head);
		tail->token = TOKEN_DOUBLE_QUOTE;
        (*i)++;  // Passer le premier "
        while (str[*i] && (str[*i] != '"'))
        {
            while (str[*i] == '"')
                (*i)++;
            
            tail->data[j++] = str[*i];
            (*i)++;
        }
		if (str[(*i) + 1] == ' ')
			tail->data[j++] = ' ';
        tail->data[j] = '\0';
        (*i)++;  // Passer le dernier "
    }
    if (str[*i] == '"' || str[*i] == 39)
        p_quotes(i, str, head);
}


/* Fonction de vérification et tokenization des quotes */
void p_quotes(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	j = 0;
	if (str[(*i)] == 39)
	{
		insert_tail(head, NULL, "TOKEN_QUOTES");
		tail = get_last_node(*head);
		tail->token = TOKEN_SIMPLE_QUOTE;
		(*i)++;
		while (str[(*i)] && (str[(*i)] != 39))
		{
			while(str[(*i)] == 39)
				(*i)++;
			tail->data[j++] = str[(*i)];
			(*i)++;
		}
		if (str[(*i) + 1] == ' ')
			tail->data[j++] = ' ';
		tail->data[j] = '\0';
		(*i)++;
	}
	if (str[(*i)] == '"')
		p_double_quotes(i, str, head);
}

struct s_shell *post_parsing_condition(struct s_shell *current, char *str, int *stop_flag)
{

	if (current->token == TOKEN_CMD && current->next->token == TOKEN_CMD)
		current->next->token = TOKEN_ARG;
	if (current->token != TOKEN_PIPE && current->next->token == TOKEN_CMD)
		current->next->token = TOKEN_ARG;
	if (current->token == TOKEN_PIPE && current->next->token == TOKEN_PIPE)
	{
		//free(str);
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
		*stop_flag = 1;
		return (NULL);
	}
	if (is_token_red(current->token) && current->next->token != TOKEN_FILE)
	{
		//free(str);
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		*stop_flag = 1;
		return (NULL);
	}
	return (current);
}

/* Second parsing corrige la succession de 2 commandes,
	succession d'une prochaine commande sans pipe  */
/* Appel récursif a parsing afin de gérer le cas ou 
	une commande n'a pas été fourni après un pipe */
struct s_shell *p_post_parsing(struct s_shell *head, char *str, struct s_shell *shell)
{
	struct s_shell *current;
	int break_flag;
	char *rl_input;

	break_flag = 0;
	current = head;
	while (current)
	{
		if (current && current->next)
		{
			current = post_parsing_condition(current, str,  &break_flag);
			if (!current)
				return (NULL);
			if (break_flag)
				break;
		}
		if (current->token == TOKEN_PIPE && !current->next)
		{
			rl_input = readline("> ");
			current = parsing(rl_input, current, shell);
		}
		current = current->next;
	}
	if (get_nb_token(head) == 1)
		head->token = TOKEN_CMD;
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

/* Parsing principale permettant de parcourir l'entièreté de la string
	et y crée une liste chainé avec les différentes valeurs et tokens */
struct s_shell *parsing(char *str, struct s_shell *head, struct s_shell *shell)
{
    int i;
	int stop_flag;

    i = 0;
	stop_flag = 0;
	//head = pre_parsing(str, head);
    while (str[i] != '\0' && !stop_flag) 
    {
        while (is_space(str[i]))
            i++;
        if (is_redirect(str[i]))
			p_redirection(&i, str, &head, &stop_flag);
		//printf("test str: %c\n", str[i]);
        if (!is_spec_char(str[i]))
            p_command(&i, str, &head, &stop_flag);
		//printf("test str: %c\n", str[i]);
        //else if (str[i] == '-')
		if (str[i])
        	p_arg(&i, str, &head);
		//printf("test str: %c\n", str[i]);
		if (is_redirect(str[i]))
			p_redirection(&i, str, &head, &stop_flag);
		else if (str[i] == 39 || str[i] == '"')
            p_quotes(&i, str, &head);
		else if (str[i] == '|')
            p_pipe(&i, str, &head);
        else if (str[i])
            i++;
		
    }
	//if (!shell->exit_code)
		//shell->exit_code = stop_flag;
	head = p_post_parsing(head, str, shell);
	if (stop_flag)
		return (NULL);
    return (head);
}
