/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:21:24 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/06 14:23:58 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
// gestion des signaux

void handle_signal(int sig, siginfo_t *info, void *context)
{
    (void)context;
    (void)info;
    
    if (sig == SIGINT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 1);
        rl_redisplay();
    }  
    else if (sig == SIGQUIT)
    {
        write(1, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 1);
        rl_redisplay();
    }
}
