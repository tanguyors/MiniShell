
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
    if ((c >= 65 && c <= 90) || c >= 97 && c <= 122)
        return (1);
    return (0);
}

int is_redirect(int c)
{
    if (c == '<' || c == '>')
        return(1);
    return(0);
}

int is_space(int c)
{
    if (c == ' ' || c == '\t')
        return (1);
    return (0);
}

struct s_shell *parsing(char *str, struct s_shell *value) 
{
    int i;
    int j;

    i = 0;
    while (str[i] != '\0') 
    {
        while (str[i] == ' ' || str[i] == '\t')
            i++;
        
        if (is_redirect(str[i])) 
        {
            insert_head(&value, NULL);
            value->token = TOKEN_WORD;
            value->data[0] = str[i++];
            value->data[1] = '\0';
            
            if (is_redirect(str[i])) 
            {
                value->data[1] = str[i++];
                value->data[2] = '\0';
            }
            if (str[++i])
            {
                insert_head(&value, NULL);
                //value = value->next;
                value->token = TOKEN_INFILE;
                j = 0;
                while (str[i] != '\0' && !is_space(str[i])) 
                {
                    value->data[j++] = str[i++];
                }
                value->data[j] = '\0';
            }
            else
                perror("syntax error near unexpected token `newline'\n");
        }
        break;
    }
    return (value);
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
