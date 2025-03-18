/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 06:53:56 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 07:23:54 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Parses through the environment to find a 
	matching variable (stored in the string 'str').
*/

void	search_var_env(t_data *data, char *str)
{
	while (data->env_head)
	{
		if (ft_strncmp(data->env_head->env, str, ft_strlen(str)) == 0)
			break ;
		data->env_head = data->env_head->next;
	}
}

/*
	Replace the argument corresponding with the environment variable
	with its actual value found in the environment.
	The string 'str' is equal to the name of the variable followed by a
	'=' character, so we can start replacing after this lenght.
*/

int	replace_var_env(t_chain **chain, t_data *data, char *str, int pos)
{
	int	start;

	start = ft_strlen(str);
	while (data->env_head->env[start])
	{
		(*chain)->content[pos] = data->env_head->env[start];
		pos++;
		start++;
	}
	return (pos);
}

//	Searches a matching variable in the environment to replace.

void	search_and_replace_var(char *str, int *j, t_data *data, t_chain **chain)
{
	int		pos;
	t_env	*tmp;

	pos = *j;
	tmp = data->env_head;
	search_var_env(data, str);
	if (data->env_head == NULL)
	{
		data->env_head = tmp;
		return ;
	}
	else
		*j = replace_var_env(chain, data, str, pos);
	data->env_head = tmp;
}

/*
	Initializes the replacement of an environment variable 
	(invoked wwith '$') by its actual value.
	To facilitates the process, this function creates a string 
	'str' that is equal to the name of the variable to replace, 
	followed by an '=' character. This string is then used to 
	parse the environment and find a matching entry.
	The first while() is used to calculate len and stop at the
	right spot, in order to malloc str.
*/

void	replace_var_env_init(char *itr_cont, int *j,
	t_data *data, t_chain **chain)
{
	int		len;
	int		tmp;
	char	*str;

	len = 0;
	tmp = data->pos;
	data->pos++;
	while (itr_cont[data->pos] && itr_cont[data->pos] != ' '
		&& itr_cont[data->pos] != '\t' && itr_cont[data->pos] != '$'
		&& itr_cont[data->pos] != '\'' && itr_cont[data->pos] != '\"')
	{
		data->pos++;
		len++;
	}
	str = malloc(sizeof(char) * (len + 2));
	if (!str)
		return ;
	len = -1;
	while (tmp < data->pos)
		str[++len] = itr_cont[++tmp];
	str[len] = '=';
	str[len + 1] = '\0';
	search_and_replace_var(str, j, data, chain);
	free(str);
}
