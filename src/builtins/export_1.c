/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 22:37:26 by acosi             #+#    #+#             */
/*   Updated: 2024/03/28 09:03:13 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_check(char **cmd, t_data *data, int i, int j)
{
	if (ft_strncmp(cmd[i], "HOME=", 5) == 0)
		redefine_home(data, cmd[i]);
	if (ft_isdigit(cmd[i][0]))
	{
		export_error(data, cmd[i]);
		return (0);
	}
	while (cmd[i][j] && cmd[i][j] != '=')
	{
		if (!ft_isalnum(cmd[i][j]))
		{
			export_error(data, cmd[i]);
			return (0);
		}
		j += 1;
	}
	if (j > 0)
		return (1);
	return (0);
}

t_env	*add_env_node(char *var)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (node == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	node->env = ft_strdup(var);
	if (node->env == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

int	redefine_var(t_env *env_list, char *cmd, int i)
{
	char	*var;

	while (cmd[i])
		i++;
	var = malloc(sizeof(char) * (i + 1));
	i = -1;
	while (cmd[++i] && cmd[i] != '=')
		var[i] = cmd[i];
	var[i] = '\0';
	if (!ft_strncmp(env_list->env, var, i))
	{
		if (cmd[i] != '=' && env_list->env[i] == '=')
		{
			free (var);
			return (1);
		}
		else if (env_list->env[i] != '=' && env_list->env[i])
		{
			free (var);
			return (0);
		}
		i = 0;
		while (cmd[i])
			i++;
		free(env_list->env);
		env_list->env = (char *)malloc(sizeof(char) * (i + 1));
		copy_var(cmd, env_list);
		free(var);
		return (1);
	}
	free(var);
	return (0);
}

void	export_2(char **cmd, t_data *data, t_env *head, int i)
{
	t_env	*new_node;

	if (!data->env_head)
	{
		new_node = add_env_node(cmd[i]);
		data->env_head = new_node;
		return ;
	}
	while (data->env_head)
	{
		if (redefine_var(data->env_head, cmd[i], 0))
		{
			data->env_head = head;
			break ;
		}
		else if (!(redefine_var(data->env_head, cmd[i], 0))
			&& !data->env_head->next)
		{
			new_node = add_env_node(cmd[i]);
			data->env_head->next = new_node;
			break ;
		}
		data->env_head = data->env_head->next;
	}
}

void	export(char **cmd, t_data *data, int i, int j)
{
	t_env	*head;
	int		equal_flag;
	t_env	*new_node;

	head = data->env_head;
	if (!data->env_head)
	{
		new_node = add_env_node(cmd[i]);
		head = new_node;
		i++;
	}
	if (!cmd[1])
		return (print_sort_env(data));
	while (cmd[++i])
	{
		equal_flag = export_check(cmd, data, i, j);
		if (equal_flag == 1)
			export_2(cmd, data, head, i);
		equal_flag = 0;
		data->env_head = head;
	}
	data->status = 0;
	if (data->builtin_error == 1)
		data->status = 1;
}
