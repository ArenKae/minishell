/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:53:46 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 13:21:58 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(char **cmd, t_data *data, t_chain *itr, t_struct **head)
{
	long int	i;

	i = data->status;
	free(data->my_env);
	free(data->home);
	free(data->last_arg);
	free(data->dir);
	free_env(data->env_head);
	free_chain(itr);
	free_list(*head);
	free(cmd);
	free(data->cmd);
	rl_clear_history();
	exit(i);
}

void	print_pwd(char **cmd, t_data *data, t_chain *itr, t_struct **head)
{
	(void)cmd;
	printf("%s\n", data->dir);
	ft_exit(cmd, data, itr, head);
}

//	Determines if a command is a builtin and returns the corresponding value.

int	is_builtin(char *command)
{
	if (command == NULL || command[0] == '\0')
		return (0);
	if (ft_strncmp(command, "env\0", 4) == 0)
		return (1);
	else if (ft_strncmp(command, "pwd\0", 4) == 0)
		return (2);
	else if (ft_strncmp(command, "echo\0", 5) == 0)
		return (3);
	else if (ft_strncmp(command, "export\0", 7) == 0)
		return (4);
	else if (ft_strncmp(command, "unset\0", 6) == 0)
		return (5);
	else if (ft_strncmp(command, "cd\0", 3) == 0)
		return (6);
	else if (ft_strncmp(command, "exit\0", 5) == 0)
		return (7);
	return (0);
}

void	exec_builtin(char **cmd, t_data *data, t_chain *itr, t_struct **head)
{
	data->builtin_error = 0;
	data->status = 0;
	if (is_builtin(cmd[0]) == 1)
		print_env(cmd, data, itr, head);
	else if (is_builtin(cmd[0]) == 2)
		print_pwd(cmd, data, itr, head);
	else if (is_builtin(cmd[0]) == 3)
		echo(cmd, data, itr, head);
	else if (is_builtin(cmd[0]) == 4)
		export(cmd, data, 0, 0);
	else if (is_builtin(cmd[0]) == 5)
		unset(cmd, data, NULL, 0);
	else if (is_builtin(cmd[0]) == 6)
		cd(cmd, data, data->my_env);
	else if (is_builtin(cmd[0]) == 7)
		exit_builtin (cmd, data, itr, head);
	if (data->pipes > 0)
		ft_exit (cmd, data, itr, head);
	free(cmd);
}
