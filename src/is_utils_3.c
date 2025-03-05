/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_utils_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:12:14 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 22:12:53 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* Détermine s'il y a un pipe dans la liste chaînée 
	auquel cas renvois le nombre de pipe total */
int is_pipe(struct s_shell *current)
{
	int n;

	n = 0;
	while (current)
	{
		if (current->token)
		{
			if (current->token == TOKEN_PIPE)
			{
				n++;
			}
		}
		current = current->next;
	}
	return (n);
}

int is_redirection_in_list(struct s_shell *head)
{
	while (head && head->token)
	{
		if (is_token_red(head->token))
			return (1);
		head = head->next;
	}
	return (0);
}