/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_utils_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:12:02 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 22:12:33 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int is_token_red(enum e_tokens token)
{
	if (token == REDIR_INPUT || token == REDIR_OUTPUT || token == REDIR_APPEND || token == REDIR_HEREDOC)
		return (1);
	return (0);
}

int is_spec_char(int c)
{
    if (c == ' ' ||  c == 39 || c == '"' || c == '|' || c == '-' || is_redirect(c))
		return (1);
	return (0);
}

int is_spec_char_no_space(int c)
{
    if (c == 39 || c == '"' || c == '|' || c == '-' || is_redirect(c))
		return (1);
	return (0);
}

int is_ignored_char(int c)
{
    if (c == '/' || c == '\\' || c == '!' || c == ':' || c == '.')
		return (1);
	return (0);
}

int is_alpha(int c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        return (1);
    return (0);
}