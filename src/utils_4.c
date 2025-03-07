/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:04:46 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/07 17:09:49 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
static size_t ft_strcspn(const char *s, const char *reject)
{
    int i;
    int j;

    i = 0;
    while (s[i])
    {
        j = 0;
        while (reject[j])
        {
            if (s[i] == reject[j])
            {
                return (i);
            }
            j++;
        }
        i++;
    }
    return (i);
}

static size_t	ft_strspn(const char *s, const char *accept)
{
    int i;
    int j;
    int check;

    i = 0;
    while (s[i])
    {
        j = 0;
        check = 0;
        while (accept[j])
        {
            if (s[i] == accept[j])
            {
                check = 1;
            }
            j++;
        }
        if (check == 0)
        {
            return (i);
        }
        i++;
    }
    return (i);
}

void update_saved_ptr(char **token_end, char **save_ptr)
{
    if (**token_end != '\0') 
    {
        **token_end = '\0';
        *save_ptr = *token_end + 1;
    } 
    else 
        *save_ptr = *token_end;
}

char *ft_strtok(char *str, const char *delim)
{
    static char *save_ptr = NULL;
    char *token_start;
    char *token_end;

    if (str == NULL && (save_ptr == NULL || *save_ptr == '\0'))
        return (NULL);
    if (str != NULL)
        save_ptr = str;
    else
    {
        str = save_ptr;
    }
    str += ft_strspn(str, delim);
    if (*str == '\0') 
    {
        save_ptr = str;
        return (NULL);
    }
    token_start = str;
    token_end = token_start + ft_strcspn(token_start, delim);
    update_saved_ptr(&token_end, &save_ptr);
    return (token_start);
}