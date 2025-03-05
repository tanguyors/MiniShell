/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:30:42 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 21:53:34 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Fonction permettant de tokenizer les pipes */
int p_pipe(int *i, char *str, struct s_shell **head)
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
	if (!str[(*i)])
		return (-1);
	return (0);
}

static void std_arg_data(int *i, char *str, struct s_shell *tail)
{
	int j;

	j = 0;
	while (str[(*i)] != '\0' && !is_spec_char_no_space(str[(*i)]))
	{
		if (is_space(str[(*i)]) && (str[(*i) + 1 ] && str[(*i) + 1 ] != '|'))
			tail->data[j++] = ' ';
		while (str[(*i)] != '\0' && is_space(str[(*i)]))
			(*i)++;
		if (!str[(*i)])
			break;
		if (is_spec_char_no_space(str[(*i)]))
			break;
		while (str[(*i)] != '\0' && is_quotes(str[(*i)]))
			(*i)++;
		if (!str[(*i)])
			break;
		if (is_spec_char_no_space(str[(*i)]))
			break;
		tail->data[j++] = str[(*i)];
		(*i)++;
	}
	tail->data[j] = '\0';
}

static void std_arg(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;
	int j;

	insert_tail(head, NULL, NULL);
	tail = get_last_node(*head);
	tail->token = TOKEN_ARG;
	std_arg_data(i, str, tail);
}

/* écrit la data de l'arg '-' dans le noeud adéquat */
static void dash_arg_data(int *i, char *str, struct s_shell *tail)
{
	int j;

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

/* Fonction permettant de tokenizer les arguments de commandes */
int p_arg(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;

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
		dash_arg_data(i, str, tail);
	}
	else if (str[(*i)] && !is_spec_char_no_space(str[(*i)]))
		std_arg(i, str, head);
	if (!str[(*i)])
		return (-1);
	return (0);
}
