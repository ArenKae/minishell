/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf42@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 07:46:46 by acosi             #+#    #+#             */
/*   Updated: 2024/03/28 09:08:30 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Malloc an copy of the environment stored in the env list
	for later use with execve(). This will be done at the start of every
	new loop in the main function.
*/

void	malloc_env_array(t_data *data, t_env *head)
{
	int		i;
	t_env	*tmp;

	i = 0;
	tmp = head;
	while (head)
	{
		i++;
		head = head->next;
	}
	data->my_env = malloc(sizeof(char *) * (i + 1));
	if (data->my_env == NULL)
		return ;
	i = 0;
	head = tmp;
	while (head)
	{
		data->my_env[i] = head->env;
		head = head->next;
		i++;
	}
	data->my_env[i] = NULL;
	head = tmp;
}

//	Creates and returns a new node containing the 
//	current line from the environment.

t_env	*create_env_node(char *env)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (node == NULL)
		return (NULL);
	node->env = ft_strdup(env);
	if (node->env == NULL)
		return (NULL);
	node->next = NULL;
	return (node);
}

//	Creates and returns a linked list where each node holds 
//	a variable from the starting environment.

t_env	*malloc_env(char **env)
{
	t_env	*head;
	t_env	*current;
	t_env	*node;
	int		i;

	i = 0;
	head = NULL;
	node = NULL;
	current = NULL;
	while (env[i])
	{
		node = create_env_node(env[i]);
		if (head == NULL)
		{
			head = node;
			current = head;
		}
		else
		{
			current->next = node;
			current = current->next;
		}
		i++;
	}
	return (head);
}

/*
	Builtin used to print the most recent environment by parsing the 
	linked list env_head in the struct data. The tmp array is used with 
	the export builtin to redefine the _ variable everytime the env 
	command is executed. The env command should not have any argument, 
	so a custom error message is displayed if that's the case.
*/

int		check_if_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	print_env(char **cmd, t_data *data, t_chain *itr, t_struct **head)
{
	t_env	*current;
	char	**tmp;

	tmp = malloc(sizeof(char *) * 3);
	tmp[0] = "";
	tmp[1] = "_=env";
	tmp[2] = NULL;
	current = data->env_head;
	export(tmp, data, 0, 0);
	free(tmp);
	if (cmd[1])
	{
		print_error_stderr(NULL, "env: Too many arguments");
		data->status = 1;
		ft_exit(cmd, data, itr, head);
	}
	while (current)
	{
		if (check_if_equal(current->env))
			printf("%s\n", current->env);
		current = current->next;
	}
	ft_exit(cmd, data, itr, head);
}

//	Stores the path to the HOME dir in the data struct for later use by cd.

void	malloc_home(t_data *data)
{
	t_env	*env;

	env = data->env_head;
	data->home = NULL;
	while (env)
	{
		if (ft_strncmp(env->env, "HOME=", 5) == 0)
		{
			data->home = ft_strdup(env->env + 5);
			if (data->home == NULL)
				return ;
			break ;
		}
		env = env->next;
	}
	if (env == NULL || env->env == NULL)
		data->home = ft_strdup("");
}
