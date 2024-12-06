#include "../include/minishell.h"

int is_spec_char(int c) 
{
    if (c == '|' || c == '-' || is_redirect(c))
		return (1);
	return (0);
}

int is_alpha(int c)
{
    if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
        return (1);
    return (0);
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
