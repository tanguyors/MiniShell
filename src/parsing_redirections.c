/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 21:27:38 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 21:02:50 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* écrit la data du token filename dans le noeud adéquat */
static void	in_out_data(char *str, int *i, struct s_shell **head)
{
	struct s_shell	*tail;
	int				j;

	insert_tail(head, NULL, "TOKEN_FILE");
	tail = get_last_node(*head);
	tail->token = TOKEN_FILE;
	j = 0;
	while (str[(*i)] != '\0' && !is_redirect(str[(*i)]) && !is_space(str[(*i)])
		&& str[(*i)] != '|')
	{
		if (is_quotes(str[(*i)]))
			(*i)++;
		else
			tail->data[j++] = str[(*i)++];
	}
	tail->data[j] = '\0';
}

/* Fonction complémentaire de p_redirection elle permet de traiter
	les fichier in file et out file et de les tokenizer */
static void	r_in_out_file(int *i, char *str, struct s_shell **head,
		int *stop_flag)
{
	while (is_space(str[(*i)]))
		(*i)++;
	if (str[(*i)])
	{
		in_out_data(str, i, head);
	}
	else
	{
		ft_putstr_fd("bash: syntax error near unexpected token `newline'\n", 2);
		*stop_flag = 1;
	}
	while (is_space(str[(*i)]))
		(*i)++;
	if (is_redirect(str[(*i)]))
		p_redirection(i, str, head, stop_flag);
}

/* Fonction permettant de déterminer le type de redirection */
enum e_tokens	which_red(int *i, char *str)
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
int	p_redirection(int *i, char *str, struct s_shell **head, int *stop_flag)
{
	struct s_shell	*tail;

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
	if (!str[(*i)])
		return (-1);
	return (0);
}
