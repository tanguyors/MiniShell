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

int is_str(char *str)
{
	while (str)
	{
		if (!is_alpha(*str))
			return (0);
		str++;
	}
	return (1);
}

int is_redirect(int c)
{
    if (c == '<' || c == '>')
        return(1);
    return(0);
}

int is_alnum(int c)
{
	if ((c >= '0' && c <= '9') || (is_alpha(c)))
		return (1);
	return (0);
}

int is_space(int c)
{
    if (c == ' ' || c == '\t')
        return (1);
    return (0);
}

int is_quotes(int c)
{
	if (c == '"' || c == 39)
		return (1);
	return (0);
}

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