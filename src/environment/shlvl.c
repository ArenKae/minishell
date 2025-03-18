/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 10:19:36 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/13 01:52:53 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*shlvl_increment(int tmp)
{
	char	*str;
	char	*value;
	char	*final_str;

	str = ft_strdup("SHLVL=");
	value = ft_itoa(tmp);
	final_str = ft_strjoin(str, value);
	free(str);
	free(value);
	return (final_str);
}

int	check_shlvl_value(char *str)
{
	int	i;

	i = 7;
	while (str[i] && (str[i] == '-' || str[i] == '+'))
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

void	redefine_shlvl(t_env *env_list, int *redefined)
{
	int		tmp;
	char	*str;

	if (ft_strncmp(env_list->env, "SHLVL=", 6) == 0)
	{
		*redefined = 1;
		if (check_shlvl_value(env_list->env))
		{
			tmp = ft_atoi(env_list->env + 6);
			if (tmp <= 0)
			{
				free(env_list->env);
				env_list->env = ft_strdup("SHLVL=0");
				return ;
			}
			tmp++;
			str = shlvl_increment(tmp);
			free(env_list->env);
			env_list->env = ft_strdup(str);
			free(str);
			return ;
		}
		free(env_list->env);
		env_list->env = ft_strdup("SHLVL=1");
	}
}

void	create_shlvl(t_data *data, t_env **new_node)
{
	char	*shlvl;

	shlvl = ft_strdup("SHLVL=1");
	(*new_node) = add_env_node(shlvl);
	data->env_head = (*new_node);
}

void	init_shlvl(t_data *data)
{
	int		redefined;
	t_env	*new_node;
	t_env	*head;

	redefined = 0;
	head = data->env_head;
	if (!data->env_head)
		return (create_shlvl(data, &new_node));
	while (data->env_head)
	{
		redefine_shlvl(data->env_head, &redefined);
		if (data->env_head->next == NULL && redefined == 0)
		{
			data->env_head->next = malloc(sizeof(t_env));
			data->env_head->next->env = ft_strdup("SHLVL=1");
			data->env_head->next->next = NULL;
			break ;
		}
		data->env_head = data->env_head->next;
	}
	data->env_head = head;
}
