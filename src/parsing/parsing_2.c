/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 13:42:47 by acosi             #+#    #+#             */
/*   Updated: 2023/11/29 14:13:46 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
    Count the number of closed quotes pairs to know how many char to malloc.
    data->i = position on the line cmd    j = size of the int used to malloc
    c stores the first quote character found by get_first_quote().
    'open' is used to keep track of the open/close status of a given pair of
	quotes. The 2nd else if () statement ensures that we can handle multiples
	pairs of joint quotes (ex : "a"'b').
	The heredoc_flag ensures we are not inside a heredoc, in which case changes 
	to the variables should not affect the environment. Otherwise, 
	var_count_replace() calculates the size of the env variable that we are 
	trying to access, if it exists. Finally, if a delimiter is found and is not 
	inside an open quote, it means we have reached the end of the parsing for 
	this node and should exit the function.
*/

void	count_quotes_parsing(char *cmd, t_data *data, int *j)
{
	int		open;

	open = 0;
	data->c = get_first_quote(&cmd[data->pos]);
	while (cmd[(data->i)])
	{
		if (cmd[data->i] == data->c && open == 1)
			open = 0;
		else if (cmd[data->i] == data->c && open == 0)
			open = 1;
		else if ((cmd[data->i] == '\'' || cmd[data->i] == '\"') && open == 0)
		{
			open = 1;
			data->c = cmd[data->i];
		}
		if (cmd[data->i] != data->c)
			*j += 1;
		if (is_delimiter_check(cmd[data->i + 1]) && open == 0)
			break ;
		data->i += 1;
	}
}

/*
	Sets up a flag if quotes are found around the heredoc delimiter.
	If that's the case, environment variables are handled differently 
	(they are not accessed, only printed).
*/

void	check_hdc_quotes(char *cmd, t_data *data, t_struct *itr)
{
	int	tmp;

	tmp = data->pos;
	while (tmp <= data->i)
	{
		if (cmd[tmp] == '\'' || cmd[tmp] == '\"')
			itr->hdc_quotes = 1;
		tmp++;
	}
}

void	get_parsing_quote(t_data *data, char *cmd, int *open)
{
	*open = 1;
	data->c = cmd[data->pos];
}

/*
	Same logic as count_quotes_parsing() : the command is parsed while 
	accounting for the quotes, but instead of calculating the size to malloc,
	the content of cmd is copied to the itr node.
	Special cases functions handle parsing inside a heredoc and accessing an 
	environment variable with the '$' character.
*/

void	args_parsing(char *cmd, t_data *data, t_struct *itr, int j)
{
	int		open;

	open = 0;
	data->c = get_first_quote(&cmd[data->pos]);
	if (data->heredoc_flag == 1)
		check_hdc_quotes(cmd, data, itr);
	while (data->pos <= data->i)
	{
		if (cmd[data->pos] == data->c && open == 1)
			open = 0;
		else if (cmd[data->pos] == data->c && open == 0)
			open = 1;
		else if ((cmd[data->pos] == '\''
				|| cmd[data->pos] == '\"') && open == 0)
			get_parsing_quote(data, cmd, &open);
		else if (data->heredoc_flag == 0 && cmd[data->pos] == '$' && (open == 0
				|| (data->c == '\"' && open == 1)))
			itr->expand = 1;
		if (cmd[data->pos] != data->c)
			itr->content[++j] = cmd[data->pos];
		data->pos++;
	}
	data->pos--;
	itr->content[j + 1] = '\0';
}
