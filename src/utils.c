
//fonctions utilitaire

#include "../include/minishell.h"

/**
 * free_array - Libère un tableau de chaînes de caractères.
 * @array: Le tableau à libérer.
 */
void free_array(char **array)
{
    int i;
    
    i = 0;
    if (!array) 
        return; // Rien à libérer
    while (array[i]) 
    {
        free(array[i]); // Libère chaque chaîne de caractères
        i++;
    }
    free(array); // Libère le tableau lui-même
}

void free_list(struct s_shell *head)
{
	struct s_shell	*tmp;
	int i;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		if (tmp->data != NULL)
			free(tmp->data);
		free(tmp);
	}
}

char *ft_strcpy(char *dest, char *src)
{
	int i;

	i = 0;
	while(src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) 
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

/**
 * trim_newline - Supprime le '\n' à la fin de la chaîne, si présent.
 * @line: La chaîne à nettoyer.
 */
void trim_newline(char *line)
{
    size_t len;

    if (!line)
        return ;
    len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') 
    {
        line[len - 1] = '\0';
    }
}

void update_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)))
    {
        setenv("OLDPWD", getenv("PWD"), 1); // Met à jour OLDPWD avec l'ancien PWD
        setenv("PWD", cwd, 1);              // Met à jour PWD avec le chemin actuel
    }
}

void exit_with_error(const char *str_error, char **array, int exit_code)
{
    if (str_error)
        ft_putstr_fd(str_error, 2);
    if (array)
        free_array(array);
    exit(exit_code);
}

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

char *ft_strtok(char *str, const char *delim)
{
    static char *save_ptr = NULL; // Le post-it magique
    char *token_start;
    char *token_end;
    // Si on a fini, on retourne NULL
    if (str == NULL && (save_ptr == NULL || *save_ptr == '\0'))
        return NULL;
    // Début de la chaîne ou suite
    if (str != NULL)
        save_ptr = str;
    else
        str = save_ptr;
    // Saute les délimiteurs au début ("   bonjour" → "bonjour")
    str += ft_strspn(str, delim);
    // Plus rien à découper ?
    if (*str == '\0') 
    {
        save_ptr = str;
        return NULL;
    }
    // Trouve la fin du token
    token_start = str;
    token_end = token_start + ft_strcspn(token_start, delim);
    // Coupe et sauvegarde
    if (*token_end != '\0') 
    {
        *token_end = '\0';
        save_ptr = token_end + 1;
    } 
    else 
    {
        save_ptr = token_end;
    }
    return token_start;
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
