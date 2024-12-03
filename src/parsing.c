
// parsing des commandes 
#include "../include/minishell.h"

/**
 * parse_command - Découpe la ligne en tokens (arguments).
 * @input: La ligne brute entrée par l'utilisateur.
 *
 * Retourne un tableau de chaînes de caractères (les arguments de la commande).
 */
char **parse_tokens(char *input) 
{
    char **tokens;
    
    if (!input || *input == '\0') 
    {
        return (NULL);
    }
    // Découpe la ligne avec ft_split, en séparant par les espaces
    tokens = ft_split(input, ' ');
    // Vérifie si la commande a été correctement découpée
    if (!tokens) 
    {
        perror("Parsing error"); // Affiche une erreur si la découpe échoue
        exit(EXIT_FAILURE);
    }
    return (tokens); // Retourne le tableau de tokens
}

int is_alpha(int c)
{
    if((c >= 65 && c <= 90) || c >= 97 && c <= 122)
        return(1);
    return(0);
}

int is_redirect(int c)
{
    if(c == '<' || c == '>')
        return(1);
    return(0);
}

void parsing(char *str, struct s_shell *value) 
{
    int i;
    int j;
    
    if (!str || *str == '\0' || !value)
        return;
    i = 0;
    while (str[i] != '\0') 
    {
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        
        if (is_redirect(str[i])) 
        {
            value->token = TOKEN_WORD;
            j = 0;
            value->data[j++] = str[i++];
            value->data[j] = '\0';
            
            if (is_redirect(str[i])) 
            {
                value->data[j++] = str[i++];
                value->data[j] = '\0';
            }
            if (value->next)
                value = value->next;
            else
                break;
             j = 0;
            while (str[i] != ' ' && str[i] != '\t' && str[i] != '\0') 
            {
                value->data[j++] = str[i++];
            }
            value->data[j] = '\0';
            value->token = TOKEN_INFILE;
        }
        if (value->next)
            value = value->next;
        else
            break;
    }
}


/* Incomplete, Parse the input to call the correct function,
    and execute the right command */
void parse_commands(char **tokens) 
{
    if (ft_strcmp(tokens[0], "pwd") == 0)
    {
        ft_pwd();
    }
    if (ft_strcmp(tokens[0], "echo") == 0)
    {
        ft_echo(tokens);
    }
    if (ft_strcmp(tokens[0], "env") == 0)
    {
        ft_env(tokens);
    }
    if (ft_strcmp(tokens[0], "export") == 0)
    {
        ft_export(tokens);
    }
    // ...
}
