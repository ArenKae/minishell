/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:10:06 by acosi             #+#    #+#             */
/*   Updated: 2023/12/14 15:16:12 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Implements the '-n' option for echo, which should not print a
	newline. This implementation with different flags ensures that 
	we can parses multiple strings of "-nnnnn" while skiping to 
	the first argument to print.
*/

void	check_echo_options(char *cmd, int *flag)
{
	int	i;

	i = 0;
	if (cmd[i] == '-')
		i++;
	if (*flag < 1 && (i == 0 || cmd[i] == '\0'))
	{
		*flag = 0;
		return ;
	}
	while (cmd[i] == 'n' && i != 0)
		i++;
	if (cmd[i] == '\0')
	{
		if (*flag == -1)
			*flag = 1;
	}
	else if (cmd[i] != '\0')
	{
		if (*flag == -1)
			*flag = 0;
		else if (*flag == 1)
			*flag = 2;
	}
}

void	echo(char **cmd, t_data *data, t_chain *itr, t_struct **head)
{
	int	i;
	int	count;
	int	flag;

	i = 1;
	flag = -1;
	count = 0;
	while (cmd[i])
	{
		check_echo_options(cmd[i], &flag);
		{
			if (flag != 1)
			{
				if (count > 0)
					ft_putchar_fd(' ', 1);
				ft_putstr_fd(cmd[i], 1);
				count++;
			}
		}
		i++;
	}
	if (flag < 1)
		ft_putchar_fd('\n', 1);
	data->status = 0;
	ft_exit(cmd, data, itr, head);
}
