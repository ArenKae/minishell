/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list_command_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 05:00:50 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 05:27:07 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	list_command_init(t_chain *chain)
{
	chain->content = NULL;
	chain->is_command = 0;
	chain->is_pipe = 0;
	chain->is_redirect_in = 0;
	chain->is_redirect_out = 0;
	chain->is_append = 0;
	chain->is_heredoc = 0;
	chain->is_var = 0;
	chain->is_empty_var = 0;
	chain->hdc_quotes = 0;
	chain->last_heredoc = 0;
	chain->command_index = 0;
}

/*
	Fills a new t_chain linked list to handle delimiters more efficiently.
	Parses the first list 'itr' to find every delimiters and add the
	corresponding flags to 'chain'.
*/

void	make_list_command_1(t_struct **itr, t_chain *chain,
	t_data *data, int *i)
{
	while ((*itr)->next && ((*itr)->is_append == 1 || (*itr)->is_pipe == 1
			|| (*itr)->is_redirect_in == 1 || (*itr)->is_redirect_out == 1))
	{
		if ((*itr)->next && (*itr)->is_redirect_in)
			chain->is_redirect_in = 1;
		else if ((*itr)->next && (*itr)->is_redirect_out)
			chain->is_redirect_out = 1;
		else if ((*itr)->next && (*itr)->is_append)
			chain->is_append = 1;
		if ((*itr)->next && (*itr)->is_pipe == 1)
		{
			data->pipes++;
			chain->is_pipe = 1;
			*i += 1;
		}
		(*itr) = (*itr)->next;
	}
}

void	make_list_command_2(t_struct **itr, t_chain **chain,
	t_data *data, int *i)
{
	(*chain)->is_heredoc = (*itr)->is_heredoc;
	(*chain)->hdc_quotes = (*itr)->hdc_quotes;
	if ((*itr)->is_heredoc == 1)
		(*chain)->heredoc = (*itr)->heredoc;
	make_list_content_init(chain, *itr, data);
	(*chain)->command_index = *i;
	data->malloc_count++;
}

void	make_list_command(t_chain **chain, t_struct **itr, t_data *data, int *i)
{
	list_command_init(*chain);
	make_list_command_1(itr, *chain, data, i);
	make_list_command_2(itr, chain, data, i);
}
