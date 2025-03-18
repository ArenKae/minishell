/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 11:17:07 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 03:01:31 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_invalid_character(char c)
{
	ft_putstr_fd("Invalid character: ", 2);
	ft_putchar_fd(c, 2);
	ft_putchar_fd('\n', 2);
}

// Checks for invalid characters and unclosed quotes in the command.

int	check_unclosed_quotes(char *cmd, t_data *data, int i, int open)
{
	while (cmd[i])
	{
		if (open == 0 && (cmd[i] == ';' || cmd[i] == '\\'))
		{
			print_invalid_character(cmd[i]);
			data->status = 2;
			return (0);
		}
		if (open == 0 && cmd[i] == '\'')
			open = 1;
		else if (open == 0 && cmd[i] == '\"')
			open = 2;
		else if (open == 1 && cmd[i] == '\'')
			open = 0;
		else if (open == 2 && cmd[i] == '\"')
			open = 0;
		i++;
	}
	if (open == 0)
		return (1);
	data->status = 2;
	print_error_stderr(NULL, "Error : Unclosed quotes");
	return (0);
}

/*
	Frees and exits of the program in case 
	an EOF is inputed (Ctrl + D signal).
*/

void	exit_on_null(t_data *data)
{
	free(data->my_env);
	free_env(data->env_head);
	free(data->cmd);
	free(data->home);
	free(data->last_arg);
	free(data->dir);
	rl_clear_history();
	ft_putstr_fd("exit\n", 1);
	exit (0);
}

/*
	Checks for errors in the command that would prevent it from 
	being interpreted. If cmd contains no unclosed quotes, it is 
	added to the commands history. flag is initialized to 0 and 
	set to 1 if the command is not empty.
*/

int	error_check(char *cmd, t_data *data, int i, int flag)
{
	if (cmd == NULL)
		exit_on_null(data);
	if (cmd[0] == '\0')
		return (0);
	if (check_unclosed_quotes(cmd, data, 0, 0) == 0)
	{
		add_history(cmd);
		return (0);
	}
	while (cmd[i])
	{
		if (cmd[i] != '\n' && (cmd[i] != ' ') && (cmd[i] != '\t'))
			flag = 1;
		i++;
	}
	if (flag == 0)
		return (0);
	return (1);
}
