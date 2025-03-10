/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:21:56 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/08 17:19:38 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	ascii_art(void)
{
	ft_printf("\n\n\n");
	ft_printf(" ███▄ ▄███▓ ██▓ ███▄    █  ██▓██░ ██ ▓█████  ██▓     ██▓    \n");
	ft_printf("▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▓██░ ██▒▓█   ▀ ▓██▒    ▓██▒    \n");
	ft_printf("▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒██▀▀██░▒███   ▒██░    ▒██░    \n");
	ft_printf("▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░▓█ ░██ ▒▓█  ▄ ▒██░    ▒██░    \n");
	ft_printf("▒██▒   ░██▒░██░▒██░   ▓██░░██░▓█▒░██▓░▒████▒░██████▒░██████▒\n");
	ft_printf("░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  ▒ ░░▒░▒░░ ▒░ ░░ ▒░▓  ░░ ▒░▓  ░\n");
	ft_printf("░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒  ▒ ░▒░ ░ ░ ░  ░░ ░ ▒  ░░ ░ ▒  ░\n");
	ft_printf("░      ░    ▒ ░   ░   ░ ░  ▒  ░  ░░ ░   ░     ░ ░     ░ ░   \n");
	ft_printf("       ░    ░           ░  ░  ░  ░  ░   ░  ░    ░  ░    ░  ░");
	ft_printf("\n\n\n");
}

static void	signals(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = &handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

static void	parse_main(struct s_shell *shell, struct s_shell *head)
{
	head = parsing(shell->rl_input, head, shell);
	if (!head)
	{
		if (shell->rl_input)
			free(shell->rl_input);
		main_2(*shell);
	}
	parse_execution(shell, head);
	if (shell->rl_input)
		free(shell->rl_input);
	if (head)
	{
		free_list(head);
	}
}

void	main_2(struct s_shell shell)
{
	struct s_shell	*head;

	signals();
	while (1)
	{
		head = NULL;
		shell.rl_input = readline("minishell> ");
		if (shell.rl_input != NULL)
			add_history(shell.rl_input);
		if (shell.rl_input == NULL)
		{
			rl_clear_history();
			free(shell.rl_input);
			free_list(head);
			clear_dir_stack(&shell);
			break ;
		}
		parse_main(&shell, head);
	}
	clear_dir_stack(&shell);
}

int	main(void)
{
	struct s_shell	shell;

	ascii_art();
	shell.exit_code = 0;
	shell.dir_stack = NULL;
	main_2(shell);
	return (0);
}
