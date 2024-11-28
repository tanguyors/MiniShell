/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:53:48 by root              #+#    #+#             */
/*   Updated: 2024/11/28 11:23:54 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

// Bibliothèques autorisées
#include <unistd.h>  // fork, execve, write, read, dup, dup2
#include <stdlib.h>  // malloc, free, getenv
#include <fcntl.h>   // open, O_CREAT, O_RDONLY
#include <errno.h>   // Gestion des erreurs (errno)
#include <sys/wait.h> // wait, waitpid
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>


//parsing.c
char **parse_command(char *input);
//utils.c
void free_array(char **array);
int ft_strcmp(const char *s1, const char *s2);
void trim_newline(char *line);
//builtins.c
int ft_echo(char **argv);


#endif
