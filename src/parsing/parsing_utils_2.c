/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 03:58:27 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 04:13:09 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Norm util used in args_parsing() to check if quotes 
	parsing ended without errors.
*/

int	valid_quotes_parsing(t_data *data, int *open)
{
	if (!data->heredoc_flag || (data->heredoc_flag && *open == 1)
		|| (data->heredoc_flag && *open == 0 && data->c != '\''
			&& data->c != '\"'))
		return (1);
	return (0);
}

//	Norm util used in args_parsing() to check if we are inside a heredoc.

int	heredoc_flag(t_data *data)
{
	if (data->heredoc_flag == 1)
	{
		data->pos++;
		return (1);
	}
	return (0);
}
