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
	if (data != NULL)
	{
		new_node->data = malloc((strlen(data) + 1) * sizeof(char));
		if (new_node->data == NULL)
		{
			write(2, "Error\n", 6);
			free(new_node);
			exit(EXIT_FAILURE);
		}
		strcpy(new_node->data, data);
	}
	else
	{
		new_node->data = malloc(256 * sizeof(char));
		if (new_node->data == NULL)
		{
			write(2, "Error\n", 6);
			free(new_node);
			exit(EXIT_FAILURE);
		}
		ft_memset(new_node->data, 0, 256);
	}
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