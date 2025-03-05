/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:52:02 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 17:52:28 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gestion de l'executions des commande
#include "../include/minishell.h"

static struct s_shell *set_first_arg(struct s_shell *head, struct s_shell *first_arg, int *flag)
{

	if (head->next->token == TOKEN_ARG && !flag)
	{
		first_arg = head;
		*flag = 1;
	}
	return (first_arg);
}

static void exec_data(struct s_shell *head, struct s_shell *shell)
{
	if (head && head->token)
	{
		if (head->token == TOKEN_CMD) // relié a TOKEN_ARG, cherche un token ARG
		{
			extract_data(shell, head);
		}
	}
}

static void exec_without_pipe(struct s_shell *shell, struct s_shell *head)
{
	struct s_shell *first_arg;
	int flag;

	flag = 0;
	first_arg = head;
	shell->file = head;
	while (head)
	{
		if (is_redirection_in_list(head))
		{
			first_arg = set_first_arg(shell, first_arg, &flag);
			if (head->token || head->next->token)
			{
				if (is_token_red(head->next->token) || is_token_red(head->token)) // relié a TOKEN_RED, cherche REDIR_INPUT, REDIR_OUTPUT, REDIR_APPEND, REDIR_HEREDOC
				{
					redirection_execution(shell, first_arg);
					head = head->next;
				}
			}
		}
		else
			exec_data(head, shell);
		head = head->next;
	}
}

/* Permet de trier les executions des commandes,
	en parcourant les tokens de la liste chaînée */
/* Passage par référence necessaire ? */
void parse_execution(struct s_shell *shell, struct s_shell *head)
{
	if (!is_pipe(head))
	{
		exec_without_pipe(shell, head);
	}
	else
	{
		multi_pipe_handling(shell, head);
	}
}
