/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:50:02 by root              #+#    #+#             */
/*   Updated: 2024/12/02 15:39:03 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main()
{
    char **read_tab;
    char *read_line;

    read_line = readline(">");
    //read_tab = parse_command(readline(">"));
    printf("test read: %s", read_line);
    
    return (0);
}
