/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:11:09 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/30 15:56:08 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_var_list(char **var, t_data *data)
{
	*var = NULL;
	data->malloc_count = 0;
}

void	make_list_command(t_chain *chain, t_struct **itr, t_data *data, int *i)
{
	list_command_init(chain);
	make_list_command_1(itr, chain, data, i);
	make_list_command_2(itr, chain, data, i);
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

	init_var_list(&var, data);
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
		make_list_command(chain, itr, data, &i);
		make_list_command_var(itr, chain, &var, &var_flag);
	}
	get_last_arg(data, var, var_flag, 1);
	return (head);
}

/*
    Main function to execute commands and decide what to do based on the content 
	of the input. Checks for errors first and displays the prompt again if 
	there is an unclosed pipe. The creation of heredocs is done inside the 
	check_final_redirect() function. Then, a new linked list t_chain is created 
	during the final parsing by make_list_command_init(). This chain will be 
	used to add relevant and more precise flags to each node in order to 
	facilitate commands interpretation. At last, the fully parsed command 
	is executed either as a solo_exec or inside a pipe. Finally, the argument 
	of the last command that was executed is retrieved by export_arg_var() 
	in order to update the '_' environment variable.
*/

void	what_to_do(t_struct **head, t_struct *itr, t_data *data, char *cmd)
{
	t_chain	*chain;
	t_chain	*chain_head;

	while (data->run == 1 && itr)
	{
		if (skip_new_line(&itr, data) == 0)
			break ;
		if (check_parsing_error1(&itr, data, 0))
			continue ;
		if (check_unclosed_pipe(itr))
			unclosed_pipe(itr, data, head);
		if (check_final_redirect(&itr, data))
			continue ;
		chain = make_list_command_init(&itr, data, 0, 0);
		chain_head = chain;
		if (!g_heredoc_signal && !data->error && data->malloc_count > 0)
		{
			if (data->pipes == 0)
				solo_exec_init(&chain, head, data, cmd);
			else if (data->pipes >= 1)
				pipex_init(&chain, head, data, cmd);
		}
		if (data->malloc_count > 0)
			export_arg_var(data, chain_head);
	}
}
