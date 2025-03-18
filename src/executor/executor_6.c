/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 16:10:39 by acosi             #+#    #+#             */
/*   Updated: 2023/11/30 15:42:33 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	make_list_content(t_chain *chain, t_struct *itr, t_data *data)
{
	int	j;

	j = 0;
	data->pos = 0;
	while (itr->content[data->pos])
	{
		if (itr->content[data->pos] == '$' && itr->expand)
			var_replace_chain(itr->content, &j, data, chain);
		else
		{
			chain->content[j] = itr->content[data->pos];
			j++;
		}
		data->pos++;
	}
	chain->content[j] = '\0';
}

void	make_list_content_init(t_chain *chain, t_struct *itr, t_data *data)
{
	int	j;

	j = 0;
	data->i = 0;
	while (itr->content[data->i])
	{
		if (itr->content[data->i] == '$' && itr->expand)
			make_heredoc_var_count(data, itr->content, &j);
		data->i++;
		j++;
	}
	if (data->empty_var == 1 && j == 1)
	{
		itr->is_empty_var = 1;
		chain->content = NULL;
		data->empty_var = 0;
		return ;
	}
	chain->content = malloc(sizeof(char) * (j + 1));
	make_list_content(chain, itr, data);
}

void	make_list_command_2(t_struct **itr,
	t_chain *chain, t_data *data, int *i)
{
	chain->is_heredoc = (*itr)->is_heredoc;
	chain->hdc_quotes = (*itr)->hdc_quotes;
	if ((*itr)->is_heredoc == 1)
		chain->heredoc = (*itr)->heredoc;
	make_list_content_init(chain, *itr, data);
	chain->command_index = *i;
	data->malloc_count++;
}
