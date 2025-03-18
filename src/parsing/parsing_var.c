/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:15:30 by keschouf          #+#    #+#             */
/*   Updated: 2024/03/28 08:11:29 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	var_count_status(int *j, t_data *data)
{
	int	nb;

	nb = data->status;
	if (nb == 0)
		*j += 1;
	else
	{
		if (nb < 0)
		{
			*j += 1;
			nb = -nb;
		}
		while (nb > 9)
		{
			nb = nb / 10;
			*j += 1;
		}
		*j += 1;
	}
	data->i += 1;
}

void	var_env_count(char *str, int *j, t_data *data)
{
	t_env	*tmp;

	tmp = data->env_head;
	while (data->env_head)
	{
		if (ft_strnstr(data->env_head->env, str, ft_strlen(str)) != NULL)
			break ;
		data->env_head = data->env_head->next;
	}
	if (data->env_head == NULL)
	{
		data->i--;
		data->env_head = tmp;
		data->empty_var = 1;
		return ;
	}
	else
		*j += (ft_strlen(data->env_head->env + ft_strlen(str)));
	data->env_head = tmp;
	data->i--;
}

void	var_env_count_init(char *cmd, int *j, t_data *data)
{
	int		len;
	int		tmp;
	char	*str;

	len = 1;
	tmp = data->i;
	data->i++;
	while (cmd[data->i] && cmd[data->i] != ' ' && cmd[data->i] != '\t'
		&& cmd[data->i] != '|' && cmd[data->i] != '>' && cmd[data->i] != '<'
		&& cmd[data->i] != '$' && cmd[data->i] != '\"' && cmd[data->i] != '\n')
	{
		data->i++;
		len++;
	}
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return ;
	len = -1;
	while (tmp < data->i)
		str[++len] = cmd[++tmp];
	str[len] = '=';
	str[len + 1] = '\0';
	var_env_count(str, j, data);
	free(str);
}

/*
	If a '$' character is found during parsing, this function will try to find
	the corresponding variable, calculate its size which will be used to malloc
	a node in the struct itr of the adequate size.
*/

void	var_count_replace(char *cmd, int *j, t_data *data)
{
	if (cmd[data->i + 1] == ' ' || cmd[data->i + 1] == '\0'
		|| cmd[data->i + 1] == '\"')
	{
		*j += 1;
		return ;
	}
	else if (cmd[data->i + 1] == '?')
		var_count_status(j, data);
	else
		var_env_count_init(cmd, j, data);
}
