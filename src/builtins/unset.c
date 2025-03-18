/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 22:04:11 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 10:39:41 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid_var(char *cmd, t_data *data)
{
	int	i;

	i = 0;
	data->status = 0;
	while (cmd[i])
	{
		if (cmd[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

void	free_var(t_data *data, t_env **tmp, t_env **head)
{
	(*tmp) = data->env_head;
	(*head) = data->env_head->next;
	data->env_head = data->env_head->next;
	free((*tmp)->env);
	free(*tmp);
}

int	var_exist(t_env *env_node, char *cmd)
{
	size_t	i;

	i = 0;
	if (env_node == NULL)
		return (0);
	while (env_node->env[i] && env_node->env[i] != '=')
		i++;
	if (ft_strncmp(env_node->env, cmd, i) == 0)
	{
		if (i == ft_strlen(cmd))
			return (1);
	}
	return (0);
}

void	unset(char **cmd, t_data *data, t_env *head, int i)
{
	t_env		*tmp;

	head = data->env_head;
	while (cmd[++i])
	{
		if (valid_var(cmd[i], data) == 0)
			continue ;
		while (data->env_head)
		{
			if (var_exist(head, cmd[i]))
			{
				free_var(data, &tmp, &head);
				break ;
			}
			else if (var_exist(data->env_head->next, cmd[i]))
			{
				tmp = data->env_head->next;
				data->env_head->next = tmp->next;
				free(tmp->env);
				free(tmp);
			}
			data->env_head = data->env_head->next;
		}
		data->env_head = head;
	}
}
