/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:52:49 by root              #+#    #+#             */
/*   Updated: 2024/12/02 19:29:46 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
// gestion des signaux

void handle_signal(int sig, siginfo_t *info, void *context)
{
    if (sig == SIGINT)
    {
        printf("> ^C\n");
    }  
    else if (sig == SIGQUIT)
        printf("> ^\\\n");
}