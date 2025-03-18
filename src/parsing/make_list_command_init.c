/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list_command_init.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 05:24:23 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 06:09:27 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Malloc's the first node of the command list.

t_chain	*make_list_malloc_first(t_chain **head)
{
	t_chain	*chain;

	chain = malloc(sizeof(t_chain));
	if (!chain)
		return (NULL);
	*head = chain;
	return (chain);
}

//	Malloc's a new node for the command list.

void	make_list_malloc(t_chain **chain)
{
	(*chain)->next = malloc(sizeof(t_chain));
	if (!(*chain)->next)
		return ;
	(*chain) = (*chain)->next;
}

/*
	Initializes the string var that will be used to store the last argument 
	of the command in order to update the _ environment variable.
	malloc_count is also initialized here to handle the first node of the list.
*/

void	init_var(char **var, t_data *data)
{
	*var = NULL;
	data->malloc_count = 0;
}

/*
	Creates a linked list t_chain from the linked list t_struct that was 
	created during pre-parsing. This new list is the result of a more detailed
	parsing that will be used to execute commands in the right order.
	First, we skip any empty environment variables in itr.
	data->malloc_count is a flag used to create the first node.
	Else each node is malloc'd and parsed to add all the relevant flags for each.
*/

t_chain	*make_list_command_init(t_struct **itr, t_data *data,
	int i, int var_flag)
{
	t_chain	*chain;
	t_chain	*head;
	char	*var;

	init_var(&var, data);
	while ((*itr))
	{
		if (data->malloc_count == 0)
			chain = make_list_malloc_first(&head);
		else if (data->malloc_count > 0 && (*itr)->is_new_line == 0)
			make_list_malloc(&chain);
		else
		{
			get_last_arg(data, var, var_flag, 0);
			chain->next = NULL;
			(*itr) = (*itr)->next;
			return (head);
		}
		make_list_command(&chain, itr, data, &i);
		make_list_command_var(itr, chain, &var, &var_flag);
	}
	get_last_arg(data, var, var_flag, 1);
	return (head);
}
