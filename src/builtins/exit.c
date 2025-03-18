/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:05:42 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 13:22:09 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long int	ft_atoi_exit(const char *str, long long int *k, int i, int j)
{
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		++i;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			j++;
		i++;
	}
	if (j == 1 && !ft_strncmp(&str[i], "9223372036854775808\0", 21))
	{
		*k = 0;
		return (1);
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		*k = *k * 10 + str[i] - '0';
		i++;
		if (*k < 0)
			return (0);
	}
	if (str[i] != '\0')
		return (0);
	if (j % 2 != 0)
		*k = *k * -1;
	return (1);
}

void	check_alpha_exit(char *str, char *value, int *err_flag)
{
	long long int	i;

	(void)i;
	i = 0;
	if (!ft_exit_isdigit(str))
	{
		*err_flag = 1;
		print_error_stderr_2("minishell: exit: ", str,
			": numeric argument required");
		i = 2;
		*value = i;
		return ;
	}
	if (!ft_atoi_exit(str, &i, 0, 0))
	{
		*err_flag = 1;
		print_error_stderr_2("minishell: exit: ", str,
			": numeric argument required");
		i = 2;
		*value = i;
		return ;
	}
	*value = i;
}

int	check_exit_arguments(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
		i++;
	if (i > 2)
	{
		print_error_stderr(NULL, "minishell: exit: too many arguments");
		return (1);
	}
	return (0);
}

void	exit_builtin(char **cmd, t_data *data, t_chain *itr, t_struct **head)
{
	char		value;
	int			err_flag;

	err_flag = 0;
	value = data->status;
	if (data->pipes == 0)
		ft_putstr_fd("exit\n", 1);
	if (cmd[1])
		check_alpha_exit(cmd[1], &value, &err_flag);
	if (err_flag == 0 && check_exit_arguments(cmd))
	{
		data->status = 1;
		return ;
	}
	free(data->my_env);
	free(data->home);
	free(data->last_arg);
	free(data->dir);
	free_env(data->env_head);
	free_chain(itr);
	free_list(*head);
	free(data->cmd);
	free(cmd);
	rl_clear_history();
	exit(value);
}

//	Exits the program and frees everything. 
//	Used when data->cmd has not been created to avoid double frees.

void	ft_exit_solo(t_data *data, t_chain *itr, t_struct **head)
{
	free(data->my_env);
	free_env(data->env_head);
	free(data->home);
	free(data->last_arg);
	free(data->dir);
	free_chain(itr);
	free(data->cmd);
	free_list(*head);
	rl_clear_history();
	exit (1);
}
