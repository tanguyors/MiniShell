/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmonsat <lmonsat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:19:20 by lmonsat           #+#    #+#             */
/*   Updated: 2025/03/05 22:19:51 by lmonsat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
