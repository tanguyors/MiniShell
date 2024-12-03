
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
    // ...
}
