/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:37:02 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 22:08:53 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Fonction permettant de tokenizer les commandes */
static int p_command(int *i, char *str, struct s_shell **head, int *stop_flag)
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
		j = 0;
		while (str[(*i)] != '\0' && !is_space(str[(*i)]) && !is_spec_char(str[(*i)]))
		{
			if (!is_space(str[(*i)]))
				tail->data[j++] = str[(*i)];
			(*i)++;
		}	
		tail->data[j] = '\0';
	}
	if (!str[(*i)])
		return (-1);
	return (0);
}

static struct s_shell *post_parsing_condition(struct s_shell *current, char *str)
{
	if (current->token == TOKEN_CMD && current->next->token == TOKEN_CMD)
		current->next->token = TOKEN_ARG;
	if (current->token != TOKEN_PIPE && current->next->token == TOKEN_CMD)
		current->next->token = TOKEN_ARG;
	if (current->token == TOKEN_PIPE && current->next->token == TOKEN_PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `|'\n", 2);
		return (NULL);
	}
	if (is_token_red(current->token) && current->next->token != TOKEN_FILE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		return (NULL);
	}
	return (current);
}

/* Second parsing corrige la succession de 2 commandes,
	succession d'une prochaine commande sans pipe  */
/* Appel récursif a parsing afin de gérer le cas ou 
	une commande n'a pas été fourni après un pipe */
static struct s_shell *p_post_parsing(struct s_shell *head, char *str, struct s_shell *shell)
{
	struct s_shell *current;
	char *rl_input;

	current = head;
	while (current)
	{
		if (current && current->next)
		{
			current = post_parsing_condition(current, str);
			if (!current)
				return (NULL);
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

/* loop parcourant la string afin de la parser et tokenizer 
	en cas d'erreur ou fin de str -1 est renvoyé */
static int parsing_loop(char *str, int *i, struct s_shell **head, int *stop_flag)
{
	if (is_redirect(str[(*i)]))
	{
		if (p_redirection(i, str, head, stop_flag) == -1)
			return (-1);
	}
	if (!is_spec_char(str[(*i)]))
	{
		if(p_command(i, str, head, stop_flag) == -1)
			return (-1);
	}
	if(p_arg(i, str, head) == -1)
		return (-1);
	if (is_redirect(str[(*i)]))
	{
		if(p_redirection(i, str, head, stop_flag) == -1)
			return (-1);
	}
	if (str[(*i)] == 39 || str[(*i)] == '"')
	{
		if (p_quotes(i, str, head) == -1)
			return (-1);
	}
	return (0);
}

/* Parsing principal loop et parse la totalité de la str return NULL en cas d'erreur */
struct s_shell *parsing(char *str, struct s_shell *head, struct s_shell *shell)
{
    int i;
	int stop_flag;

    i = 0;
	stop_flag = 0;
    while (str[i] != '\0' && !stop_flag) 
    {
        while (str[i] && is_space(str[i]))
            i++;
		if (parsing_loop(str, &i, &head, &stop_flag) == -1)
			break;
		if (str[i] == '|')
		{
			if(p_pipe(&i, str, &head) == -1)
				break;
		}
		else if (str[(i)])
    		i++;
    }
	head = p_post_parsing(head, str, shell);
	if (stop_flag)
		return (NULL);
    return (head);
}

