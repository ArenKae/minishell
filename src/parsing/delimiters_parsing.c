/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiters_parsing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:02:45 by acosi             #+#    #+#             */
/*   Updated: 2023/12/04 15:41:52 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Parses and malloc a new node for pipe characters.

void	pipe_parsing(char *cmd, t_data *data, t_struct *itr)
{
	int	i;
	int	j;

	i = 0;
	j = data->pos - 1;
	itr->is_pipe = 1;
	while (cmd[++j] == '|')
		i++;
	if (i >= 1)
		itr->is_pipe = 1;
	itr->content = NULL;
}

// Parses and malloc a new node for '>' characters.

void	greater_parsing(char *cmd, t_data *data, t_struct *itr)
{
	int	i;
	int	j;

	i = 0;
	j = data->pos - 1;
	while (cmd[++j] == '>')
		i++;
	if (i == 1)
		itr->is_redirect_out = 1;
	else if (i >= 2)
	{
		itr->is_append = 1;
		data->pos++;
	}
	itr->content = NULL;
}

// Parses and malloc a new node for '<' characters.

void	lesser_parsing(char *cmd, t_data *data, t_struct *itr)
{
	int	i;
	int	j;

	i = 0;
	j = data->pos - 1;
	while (cmd[++j] == '<')
		i++;
	if (i == 1)
		itr->is_redirect_in = 1;
	else if (i >= 2)
	{
		itr->is_heredoc = 1;
		if (data->heredoc_seen == 1)
			data->heredoc_pos = -1;
		else
			data->heredoc_pos = data->pos;
		data->heredoc_seen = 1;
		data->heredoc_flag = 1;
		data->pos++;
	}
	itr->content = NULL;
}

// Special case parsing when a delimiter is encountered.
// Delimiters are : |, >, or <.

void	delimiters_parsing(char *cmd, t_struct *itr, t_data *data)
{
	data->heredoc_flag = 0;
	if (cmd[data->pos] == '|')
	{
		pipe_parsing(cmd, data, itr);
		data->pipe_flag = 1;
	}
	else
	{
		if (cmd[data->pos] == '>')
			greater_parsing(cmd, data, itr);
		else if (cmd[data->pos] == '<')
			lesser_parsing(cmd, data, itr);
	}
	if (data->heredoc_flag == 0 && itr->is_pipe == 0)
		itr->delimiter_token = 1;
}

//	Parsing util for init_args_parsing() in case of empty quote arg

void	empty_quote(t_struct *itr, t_data *data)
{
	itr->content = ft_strdup("");
	data->pos = data->i;
}
