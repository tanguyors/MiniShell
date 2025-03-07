/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:30:52 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/06 15:48:14 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Fonction complémentaire a p_quotes, permet de vérifier s'il y a des double quotes
	et tokenize en conséquence */
static void p_double_quotes(int *i, char *str, struct s_shell **head)
{
    int j = 0;

    if (str[*i] == '"')
    {
		insert_tail(head, NULL, "TOKEN_QUOTES");
		struct s_shell *tail = get_last_node(*head);
		tail->token = TOKEN_DOUBLE_QUOTE;
        (*i)++;
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
        (*i)++;
    }
    if (str[*i] == '"' || str[*i] == 39)
        p_quotes(i, str, head);
}

/* écrit la data des quotes dans le noeud adéquat */
static void quotes_data(int *i, char *str, struct s_shell *tail)
{
	int j;

	j = 0;
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

/* Fonction de vérification et tokenization des quotes */
int p_quotes(int *i, char *str, struct s_shell **head)
{
	struct s_shell *tail;

	if (str[(*i)] == 39)
	{
		insert_tail(head, NULL, "TOKEN_QUOTES");
		tail = get_last_node(*head);
		tail->token = TOKEN_SIMPLE_QUOTE;
		quotes_data(i, str, tail);
	}
	if (str[(*i)] == '"')
		p_double_quotes(i, str, head);
	if (!str[(*i)])
		return (-1);
	return (0);
}
