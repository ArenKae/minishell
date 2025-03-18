/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_list_command_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 16:10:39 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 00:52:05 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quote_inside_quote(t_data *data, t_struct *itr, int *open)
{
	*open = 1;
	data->c = itr->content[data->pos];
}

/*
	Definitive quote parsing for the command to execute. The logic is 
	similar as for the previous heredoc quote parsing.
	This time, we account for the '$' character to handles environment 
	variables. If a '$' is encountered, replace_var() will attempt 
	to replace it byt the value of the corresponding variable inside 
	our environment.
*/

void	quotes_list(t_chain **chain, t_struct *itr, t_data *data, int *j)
{
	int	open;

	open = 0;
	data->pos = 0;
	data->c = get_first_quote(&(itr->content[data->pos]));
	while (data->pos < data->i)
	{
		if (itr->content[data->pos] == data->c && open == 1)
			open = 0;
		else if (itr->content[data->pos] == data->c && open == 0)
			open = 1;
		else if ((itr->content[data->pos] == '\''
				|| itr->content[data->pos] == '\"') && open == 0)
			quote_inside_quote(data, itr, &open);
		data->open_quote = open;
		if (itr->content[data->pos] == '$'
			&& (open == 0 || (open == 1 && data->c == '\"')))
			replace_var(itr->content, j, data, chain);
		else if (itr->content[data->pos] != data->c)
		{
			(*chain)->content[*j] = itr->content[data->pos];
			*j += 1;
		}
		data->pos++;
	}
}

void	quotes_list_count(t_struct *itr, t_data *data, int *j)
{
	int		open;

	open = 0;
	data->c = get_first_quote(&itr->content[data->i]);
	while (itr->content[(data->i)])
	{
		if (itr->content[data->i] == data->c && open == 1)
			open = 0;
		else if (itr->content[data->i] == data->c && open == 0)
			open = 1;
		else if ((itr->content[data->i] == '\''
				|| itr->content[data->i] == '\"') && open == 0)
		{
			open = 1;
			data->c = itr->content[data->i];
		}
		if (itr->content[data->i] == '$' && (open == 0
				|| (open == 1 && data->c == '\"')))
			make_var_count(data, itr->content, j, open);
		else if (itr->content[data->i] != data->c)
			*j += 1;
		data->i += 1;
	}
}

void	make_list_content(t_chain **chain, t_struct *itr, t_data *data)
{
	int	j;

	j = 0;
	data->pos = 0;
	while (data->pos < data->i)
	{
		quotes_list(chain, itr, data, &j);
		data->pos++;
	}
	(*chain)->content[j] = '\0';
}

void	make_list_content_init(t_chain **chain, t_struct *itr, t_data *data)
{
	int	j;

	j = 0;
	data->i = 0;
	while (itr->content[data->i])
	{
		quotes_list_count(itr, data, &j);
		j++;
	}
	if (data->empty_var == 1 && j == 1)
	{
		itr->is_empty_var = 1;
		(*chain)->content = NULL;
		data->empty_var = 0;
		return ;
	}
	(*chain)->content = malloc(sizeof(char) * (j + 1));
	make_list_content(chain, itr, data);
}
