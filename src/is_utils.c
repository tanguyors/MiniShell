/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:22:24 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 19:02:02 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_str(char *str)
{
	while (str)
	{
		if (!is_alpha(*str))
			return (0);
		str++;
	}
	return (1);
}

int	is_redirect(int c)
{
	if (!c)
		return (0);
	if (c == '<' || c == '>')
		return (1);
	return (0);
}

int	is_alnum(int c)
{
	if ((c >= '0' && c <= '9') || (is_alpha(c)))
		return (1);
	return (0);
}

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	is_quotes(int c)
{
	if (c == '"' || c == 39)
		return (1);
	return (0);
}
