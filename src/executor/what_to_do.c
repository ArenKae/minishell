/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   what_to_do.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:11:09 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/19 06:48:36 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    Main function to execute commands and decide what to do based on the content 
	of the input. Checks for errors first and displays the prompt again if 
	there is an unclosed pipe. The creation of heredocs is done inside the 
	check_final_redirect() function. Then, a new linked list t_chain is created 
	for the final parsing by make_list_command_init(). This chain will be 
	used to add relevant and more precise flags to each node in order to 
	facilitate commands interpretation. At last, the fully parsed command 
	is executed either as a solo_exec or inside a pipe. Finally, the argument 
	of the last command that was executed is retrieved by export_arg_var() 
	in order to update the '_' environment variable.

	The flag data->run is set to 0 when the command list is fully created,
	to signal that we should stop the execution.
*/

void	what_to_do(t_struct **head, t_struct *itr, t_data *data, char *cmd)
{
	t_chain	*chain;
	t_chain	*chain_head;

	while (data->run == 1 && itr)
	{
		if (skip_new_line(&itr, data) == 0)
			break ;
		if (check_parsing_error(&itr, data, 0))
			continue ;
		if (check_unclosed_pipe(itr))
			unclosed_pipe(itr, data, head);
		if (check_final_redirect(&itr, data))
			continue ;
		chain = make_list_command_init(&itr, data, 0, 0);
		chain_head = chain;
		if (!g_signal && !data->error && data->malloc_count > 0)
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
