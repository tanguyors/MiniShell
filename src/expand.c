/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:22:39 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 22:22:42 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*expand_variable(const char *var)
{
	size_t	len;
	char	*var_name;
	char	*value;

	len = 0;
	while (var[len] && (ft_isalnum(var[len]) || var[len] == '_'))
		len++;
	var_name = ft_substr(var, 0, len);
	value = getenv(var_name);
	free(var_name);
	if (value)
		return (value);
	return ("");
}
static void	expand_exit_code(const char *input, size_t *i, char *result,
		size_t *j, size_t max_len, struct s_shell *shell)
{
	char	*code_str;
	size_t	k;

	code_str = ft_itoa(shell->exit_code);
	k = 0;
	if (code_str)
	{
		while (code_str[k] && ((*j) + 1) < max_len)
		{
			result[(*j)++] = code_str[k];
			k++;
		}
		free(code_str);
	}
	*i += 2;
}

static void	expand_var_name(const char *input, size_t *i, char *result,
		size_t *j, size_t max_len)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*var_value;
	size_t	k;

	var_start = *i;
	while ((input[*i] >= '0' && input[*i] <= '9') || (input[*i] >= 'A'
			&& input[*i] <= 'Z') || (input[*i] >= 'a' && input[*i] <= 'z')
		|| input[*i] == '_')
		(*i)++;
	var_len = *i - var_start;
	var_name = ft_substr(input, var_start, var_len);
	if (!var_name)
		return ;
	var_value = getenv(var_name);
	free(var_name);
	if (!var_value)
		var_value = "";
	k = 0;
	while (var_value[k] && ((*j) + 1) < max_len)
	{
		result[(*j)++] = var_value[k];
		k++;
	}
}

char	*expand_token(const char *input, int is_in_single_quote,
		struct s_shell *shell)
{
	size_t	input_len;
	size_t	max_len;
	char	*result;
	size_t	i;
	size_t	j;

	if (!input)
		return (NULL);
	if (is_in_single_quote)
		return (ft_strdup(input));
	input_len = ft_strlen(input);
	max_len = input_len * 2 + 1;
	result = malloc(max_len);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '$')
		{
			if (input[i + 1] == '?')
				expand_exit_code(input, &i, result, &j, max_len, shell);
			else
			{
				i++; /* Skip the '$' */
				expand_var_name(input, &i, result, &j, max_len);
			}
		}
		else
		{
			if ((j + 1) < max_len)
				result[j++] = input[i];
			i++;
		}
	}
	result[j] = '\0';
	return (result);
}
