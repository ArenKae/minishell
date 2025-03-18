/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   underscore.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 06:08:27 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 06:26:25 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Copy the content of the last node of the command list inside var.
	This string will be used by get_last_arg to update the _ environment 
	variable later.
	If the command ends with '$_', the _ environment variable must 
	not be updated (this is the default behaviour of bash). Hence, 
	we create a var_flag to check for this condition.
*/

void	make_list_command_var(t_struct **itr, t_chain *chain,
	char **var, int *var_flag)
{
	if (chain->content != NULL && !chain->is_empty_var)
		*var = chain->content;
	if (!ft_strncmp((*itr)->content, "$_\0", 3))
		*var_flag = 1;
	else
		*var_flag = 0;
	(*itr) = (*itr)->next;
	chain->next = NULL;
}

/*
	Copy the string 'var' inside data->last_arg while creating the command list. 
	'var' the last argument of the current line, which will be used to update 
	the value of the '_' environment variable.
	If end_flag == 1 at the end of the command list, data->run is set to 0
	to signal the what_to_do() function to the execution.
*/

void	get_last_arg( t_data *data, char *var, int var_flag, int end_flag)
{
	if (var_flag == 1)
		return ;
	if (!var)
	{
		if (data->last_arg != NULL)
			free(data->last_arg);
		data->last_arg = NULL;
		return ;
	}
	if (data->last_arg != NULL)
		free(data->last_arg);
	data->last_arg = ft_strdup(var);
	if (end_flag == 1)
		data->run = 0;
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

//	After a command was executed (with or without success), its last argument 
//	is retrieved and stored inside the '_' environment variable.

void	export_arg_var(t_data *data, t_chain *chain_head)
{
	char	*full_var;

	if (data->last_arg == NULL)
		return (free_chain(chain_head));
	full_var = ft_strjoin("_=", data->last_arg);
	export_underscore(full_var, data);
	free(full_var);
	free_chain(chain_head);
}
