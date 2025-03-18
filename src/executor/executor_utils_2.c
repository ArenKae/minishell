/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:45:15 by keschouf          #+#    #+#             */
/*   Updated: 2023/11/29 14:39:24 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_new_line(t_struct **itr, t_data *data)
{
	data->pipes = 0;
	while ((*itr) && (*itr)->is_new_line == 1)
	{
		if ((*itr)->next == NULL)
			return (0);
		*itr = (*itr)->next;
	}
	return (1);
}

void	redefine_underscore(char *var, t_env *env_list, int *redefined)
{
	if (ft_strncmp(env_list->env, "_=", 2) == 0)
	{
		free(env_list->env);
		env_list->env = ft_strdup(var);
		*redefined = 1;
	}
}

// Creates a new env in case it has been completely unset.

void	update_env(t_data *data, t_env **new_node, char *var)
{
	(*new_node) = add_env_node(var);
	data->env_head = (*new_node);
	free(data->my_env);
	malloc_env_array(data, data->env_head);
}

void	export_underscore(char *var, t_data *data)
{
	int		redefined;
	t_env	*new_node;
	t_env	*head;

	redefined = 0;
	head = data->env_head;
	if (!data->env_head)
		return (update_env(data, &new_node, var));
	while (data->env_head)
	{
		redefine_underscore(var, data->env_head, &redefined);
		if (redefined == 0 && !data->env_head->next)
		{
			new_node = add_env_node(var);
			data->env_head->next = new_node;
		}
		data->env_head = data->env_head->next;
	}
	data->env_head = head;
	free(data->my_env);
	malloc_env_array(data, data->env_head);
}

int	check_empty_var(t_struct **itr)
{
	if ((*itr)->is_empty_var == 1)
	{
		*itr = (*itr)->next;
		return (1);
	}
	return (0);
}
