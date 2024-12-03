
#include "../include/minishell.h"

void print_list(struct s_shell *current)
{
	ft_printf("Liste chainé : ");
	while (current != NULL)
	{
		ft_printf("%s -> ", current->data);
		current = current->next;
	}
	ft_printf("NULL\n");
}

struct s_shell	*create_node(char *data)
{
	struct s_shell	*new_node;

	new_node = malloc(sizeof(struct s_shell));
	if (new_node == NULL)
	{
		write(2, "Error\n", 6);
		exit(EXIT_FAILURE);
	}
	ft_memset(new_node, 0, sizeof(struct s_shell));
    if (data == NULL)
	{
		new_node->data = malloc(256 * sizeof(char));
		if (new_node->data == NULL)
		{
			write(2, "Error\n", 6);
			free(new_node);
			exit(EXIT_FAILURE);
		}
	}
    else
	    new_node->data = data;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void insert_head(struct s_shell **head, char *new_data)
{
	struct s_shell	*new_node;

	new_node = create_node(new_data);
	new_node->next = *head;
	if (*head != NULL)
	{
		(*head)->prev = new_node;
	}
	*head = new_node;
}

int main(void)
{
    struct s_shell value;
    struct s_shell *head;
    struct sigaction sa;
    char **tokens;
    
    sa.sa_sigaction = &handle_signal;
	sigemptyset(&sa.sa_mask);
    //sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    while (1)
    {
        int i;
        char *input;
        
        head = NULL;
        i = 0;
        input = readline("> ");
        //tokens = parse_tokens(input);   //  <-- a décommenter pour tes tests
        
        /* temporary exit */
        /*if(tokens && ft_strcmp(tokens[0], "exit") == 0) //  <-- a décommenter pour tes tests
            exit(EXIT_SUCCESS);*/

        /*while(tokens && tokens[i])  //  <-- a décommenter pour tes tests
        {
            printf("token [%d]: %s\n", i, tokens[i]);
            i++;
        }*/
        /*while(i--)   // <-- a décommenter pour tes tests
            insert_head(&head, tokens[i]);*/
        //parse_commands(tokens);
        head = parsing(input, head);
        //ft_echo(tokens);
        //ft_pwd();
        if (head)
        {
            print_list(head);
            free_list(head);
        }
    }
    return (0);
}
