
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

void exit_with_error(const char *str_error, char **array)
{
    if (str_error)
        perror(str_error);
    if (array)
        free_array(array);
    exit(EXIT_FAILURE);
}