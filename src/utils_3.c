/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:14:48 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/06 16:16:28 by lmonsat          ###   ########.fr       */
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

char *check_strtok(char **str, char *save_ptr)
{
    if (str == NULL && (save_ptr == NULL || *save_ptr == '\0'))
        return (NULL);
    if (str != NULL)
        save_ptr = str;
    else
    {
        str = save_ptr;
    }
    return (save_ptr);
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
    if (*token_end != '\0') 
    {
        *token_end = '\0';
        save_ptr = token_end + 1;
    } 
    else 
        save_ptr = token_end;
    return (token_start);
}

char *ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    i = 0;
    while (src[i] && i < n)
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return (dest);
}

char *ft_strcat(char *dest, const char *src)
{
    size_t dest_len;
    size_t i;

    dest_len = ft_strlen(dest);
    i = 0;
    while (src[i] != '\0')
    {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
    return dest;
}
