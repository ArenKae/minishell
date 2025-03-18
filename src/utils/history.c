/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 16:53:35 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/20 13:22:13 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	should_add_2(char *cmd, int *start, int *pipe_flag, int *new_line)
{
	if (cmd[*start] != '\n')
		*new_line = 0;
	if (cmd[*start] == '|' && *pipe_flag == 1)
		*pipe_flag = 0;
	else if (cmd[*start] == '|' && *pipe_flag == 0)
		*pipe_flag = 2;
	else if (cmd[*start] != ' ' && cmd[*start] != '\n' && *pipe_flag == 0)
		*pipe_flag = 1;
	*start += 1;
}

int	should_add(char *cmd, t_data *data)
{
	int	pipe_flag;
	int	new_line;
	int	start;

	start = data->start;
	pipe_flag = 1;
	new_line = 1;
	while (start < data->pos)
	{
		if (cmd[start] == '#')
		{
			while (cmd[start] != '\n' && cmd[start] != '\0')
				start++;
			if (cmd[start] == '\0' && pipe_flag == 1)
				return (1);
			else if (cmd[start] == '\0' && pipe_flag == 0)
				return (0);
			pipe_flag = 0;
			continue ;
		}
		should_add_2(cmd, &start, &pipe_flag, &new_line);
	}
	if (new_line == 1)
		return (-1);
	return (pipe_flag);
}

/*
	Adds the command to the history.
	First, a flag is created to determine if the current line should be added 
	or not (for example, empty lines composed of spaces or tabs should not be 
	added). Then, a string is malloc'd to store the content of the line in order 
	to use it with add_history. If a newline is found at this point, it means we 
	are in an unclosed pipe (because newline history is handled separately in 
	parsing_and_malloc). The \n is then transformed into a space, and the 
	data->full_cmd is stored to recompose the full command and add it to the 
	history later during unclosed pipe parsing.
*/

void	make_add_history(char *cmd, t_data *data, int i, int flag)
{
	flag = should_add(cmd, data);
	if (flag >= 0)
	{
		data->full_cmd = malloc(sizeof(char *) * (data->pos - data->start + 1));
		while (data->start < data->pos)
		{
			if (cmd[data->start] == '\n')
				data->full_cmd[i] = ' ';
			else
				data->full_cmd[i] = cmd[data->start];
			data->start++;
			i++;
		}
		data->full_cmd[i] = '\0';
		if (flag > 0)
		{
			add_history(data->full_cmd);
			free(data->full_cmd);
		}
	}
	data->start = data->pos + 1;
	if (data->heredoc_seen == 1)
		data->add_history = 0;
}

/*
	Parsing util that add the current line to the history when 
	a newline character is found. Also sets the appropriate 
	flag in the corresponding itr struct node.
*/

void	newline_history(char *cmd, t_struct **itr, t_data *data)
{
	make_add_history(cmd, data, 0, 0);
	(*itr)->is_new_line = 1;
}
