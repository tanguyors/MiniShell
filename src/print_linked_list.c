/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_linked_list.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:19:03 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 20:54:05 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* fonction de test */
/*void	print_list(struct s_shell *current)
{
	ft_printf("Liste chainé : ");
	while (current != NULL)
	{
		if (current->data)
			ft_printf("%s -> ", current->data);
		current = current->next;
	}
	ft_printf("NULL\n");
}*/

/* fonction de test */
/*const char	*get_token_name(enum e_tokens token)
{
	switch (token)
	{
	case TOKEN_UNDEFINED:
		return ("TOKEN_UNDEFINED");
	case TOKEN_CMD:
		return ("TOKEN_CMD");
	case TOKEN_ARG:
		return ("TOKEN_ARG");
	case TOKEN_RED:
		return ("TOKEN_RED");
	case REDIR_INPUT:
		return ("REDIR_INPUT");
	case REDIR_OUTPUT:
		return ("REDIR_OUTPUT");
	case REDIR_APPEND:
		return ("REDIR_APPEND");
	case REDIR_HEREDOC:
		return ("REDIR_HEREDOC");
	case TOKEN_PIPE:
		return ("TOKEN_PIPE");
	case TOKEN_FILE:
		return ("TOKEN_FILE");
	case TOKEN_SIMPLE_QUOTE:
		return ("TOKEN_SIMPLE_QUOTE");
	case TOKEN_DOUBLE_QUOTE:
		return ("TOKEN_DOUBLE_QUOTE");
	default:
		return (NULL);
	}
}*/

/* fonction de test */
/*void	print_token(struct s_shell *current)
{
	ft_printf("Liste token : ");
	while (current != NULL)
	{
		ft_printf("%s -> ", get_token_name(current->token));
		current = current->next;
	}
	ft_printf("NULL\n");
}*/

int	get_nb_token(struct s_shell *current)
{
	int	i;

	i = 0;
	while (current != NULL)
	{
		current = current->next;
		i++;
	}
	return (i);
}
