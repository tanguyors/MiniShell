#include "../include/minishell.h"

/* fonction de test */
void print_list(struct s_shell *current)
{
	ft_printf("Liste chainé : ");
	while (current != NULL)
	{
		if (current->data)
			ft_printf("%s -> ", current->data);
		current = current->next;
	}
	ft_printf("NULL\n");
}

/* fonction de test */
const char *get_token_name(enum e_tokens token) 
{
   switch (token) 
   {
      case TOKEN_UNDEFINED: return "TOKEN_UNDEFINED";
      case TOKEN_CMD: return "TOKEN_CMD";
	  case TOKEN_ARG: return "TOKEN_ARG";
	  case TOKEN_RED: return "TOKEN_RED";
	  case REDIR_INPUT: return "REDIR_INPUT";
	  case REDIR_OUTPUT: return "REDIR_OUTPUT";
	  case REDIR_APPEND: return "REDIR_APPEND";
	  case REDIR_HEREDOC: return "REDIR_HEREDOC";
	  case TOKEN_PIPE: return "TOKEN_PIPE";
	  case TOKEN_FILE: return "TOKEN_FILE";
	  case TOKEN_SIMPLE_QUOTE: return "TOKEN_SIMPLE_QUOTE";
	  case TOKEN_DOUBLE_QUOTE: return "TOKEN_DOUBLE_QUOTE";
	  default: return (NULL);
   }
}

/* fonction de test */
void print_token(struct s_shell *current)
{
	ft_printf("Liste token : ");
	while (current != NULL)
	{
		ft_printf("%s -> ", get_token_name(current->token));
		current = current->next;
	}
	ft_printf("NULL\n");
}

int get_nb_token(struct s_shell *current)
{
	int i;

	i = 0;
	while (current != NULL)
	{
		current = current->next;
		i++;
	}
	return (i);
}

/* Fonction complémentaire de create_node 
	permet de set la size ainsi que d'initialiser data, 
	si necessaire*/
static void data_node(struct s_shell *new_node, char *token_type)
{
	int size;

	size = 255;
	if (token_type)
	{
		if (strcmp(token_type, "TOKEN_RED") == 0)
			size = 3;
		else if (strcmp(token_type, "TOKEN_FILE") == 0)
			size = NAME_MAX;
		else if (strcmp(token_type, "TOKEN_QUOTES") == 0)
			size = ARG_MAX;
	}
	new_node->data = malloc(size * sizeof(char));
	if (new_node->data == NULL)
	{
		write(2, "Error\n", 6);
		free(new_node);
		exit(EXIT_FAILURE);
	}
	ft_memset(new_node->data, 0, size);
}

struct s_shell	*create_node(char *data, char *token_type)
{
	struct s_shell	*new_node;
	new_node = malloc(sizeof(struct s_shell));
	if (new_node == NULL)
	{
		write(2, "Error\n", 6);
		exit(EXIT_FAILURE);
	}
	ft_memset(new_node, 0, sizeof(struct s_shell));
	if (data != NULL)
	{
		new_node->data = malloc((ft_strlen(data) + 1) * sizeof(char));
		if (new_node->data == NULL)
		{
			write(2, "Error\n", 6);
			free(new_node);
			exit(EXIT_FAILURE);
		}
		ft_strcpy(new_node->data, data);
	}
	else
		data_node(new_node, token_type);
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

/* Insert un noeud au début de la liste donnée
	le token_type peut être précisé afin d'optimisé la taille de size 
	si token_type == NULL alors data n'est pas initialisé */
void insert_head(struct s_shell **head, char *new_data, char *token_type)
{
	struct s_shell	*new_node;

	new_node = create_node(new_data, token_type);
	new_node->next = *head;
	if (*head != NULL)
	{
		(*head)->prev = new_node;
	}
	*head = new_node;
}

/* Retourne le dernier noeud de la liste donnée */
struct s_shell *get_last_node(struct s_shell *head)
{
    if (head == NULL)
        return NULL;
    while (head->next != NULL)
        head = head->next;
    return (head);
}

/* Insert un noeud a la fin de la liste donnée
	le token_type peut être précisé afin d'optimisé la taille de size 
	si token_type == NULL alors data n'est pas initialisé */
void insert_tail(struct s_shell **head, char *new_data, char *token_type)
{
    struct s_shell *new_node;
    struct s_shell *last_node;

    new_node = create_node(new_data, token_type);
	new_node->next = NULL;
    if(*head == NULL)
         *head = new_node;
    else
    {
        last_node = *head;
        while(last_node->next != NULL)
        {
            last_node = last_node->next;
        }
        last_node->next = new_node;
    }
}
