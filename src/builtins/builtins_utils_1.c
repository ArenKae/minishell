/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 17:02:39 by acosi             #+#    #+#             */
/*   Updated: 2023/12/08 16:27:29 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Export util.

void	copy_var(char *cmd, t_env *env_list)
{
	int	i;

	i = -1;
	while (cmd[++i])
		env_list->env[i] = cmd[i];
	env_list->env[i] = '\0';
}

//	Export util.

void	redefine_home(t_data *data, char *cmd)
{
	free(data->home);
	if (cmd == NULL)
		data->home = ft_strdup("");
	else
		data->home = ft_strdup(cmd + 5);
}

int	ft_exit_isdigit_2(char c, int *len)
{
	if (c >= '0' && c <= '9')
	{
		*len += 1;
		return (1);
	}
	else if (c == '+' || c == '-' || c == ' ' || c == '\t'
		|| c == '\n' || c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

//	Exit util.

int	ft_exit_isdigit(char *str)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (!ft_exit_isdigit_2(str[i], &len))
			return (0);
		i++;
	}
	if (len > 21)
		return (0);
	return (1);
}

void	export_error(t_data *data, char *var)
{
	print_error_stderr_2("minishell: export: `",
		var, "': not a valid identifier");
	data->builtin_error = 1;
	return ;
}
