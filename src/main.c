/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 14:20:59 by acosi             #+#    #+#             */
/*   Updated: 2023/12/20 13:22:23 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal;

/*
	Parses the string cmd and decides what to do with it depending on its 
	content. First, a structure itr is created to split the command into 
	nodes, and add some pre-parsing data to it. Some variables from the 
	struct data need to be reinitialized with each new command, so the 
	reinit_data() function will do it on every loop. If errors are 
	detected in the command, the program stops there and starts a new 
	loop. If there are no errors, the parsing starts and the program 
	will try to interpret the command.
*/

void	init_minishell(char *cmd, t_data *data)
{
	t_struct	*head;
	t_struct	*itr;

	reinit_data(data);
	if (error_check(cmd, data, 0, 0) == 0)
		return ;
	head = malloc(sizeof(t_struct));
	itr = head;
	init_parsing(cmd, itr, data);
	if (g_signal == CTRL_C || g_signal == CTRL_C_HEREDOC)
		data->status = 130;
	else if (g_signal == CTRL_D)
		data->status = 0;
	g_signal = 0;
	if (data->count == 0)
		return (free_list(head));
	what_to_do(&head, itr, data, cmd);
	free_list(head);
}

/*	
	Initializes the environment by creating a copy of env in the struct data.
	Also creates a copy of the HOME directory in case it is unset later.
	If no environement is transmited, do nothing.
*/

void	init_env(t_data *data, char **env)
{
	if (env == NULL)
		return ;
	else
	{
		data->env_head = malloc_env(env);
		malloc_home(data);
	}
}

/*
	At the start of the program, the current environment is stored in a 
	linked list env_head inside the data struct. Then, the path to the 
	HOME directory is saved in data with the malloc_home() function. This 
	is useful if HOME is unset at some point to ensure that cd will continue 
	to work. Most variables from the data structure are initialized before 
	the minishell loop begins. A while(1) loop is used to ensure that the 
	program will keep running unless a signal is sent to exit. Inside the 
	loop, a signals() function is responsible for catching Ctrl+c and 
	Ctrl+\ signals. Then, the env_head linked list will be parsed and 
	copied inside an env_array stored inside data. This copy of the 
	environement will be used by execve() to execute commands.
	readline() will display a new prompt and store the input inside a string cmd.
	Finally, the init_minishell() function interprets the content of cmd before 
	freeing it and the environment to start a new loop.
*/

int	main(int ac, char **av, char **env)
{
	char	*cmd;
	t_data	data;

	(void)ac;
	(void)av;
	init_env(&data, env);
	init_data(&data);
	init_shlvl(&data);
	while (1)
	{
		signals();
		malloc_env_array(&data, data.env_head);
		data.add_history = 0;
		cmd = readline("minishell$ ");
		data.cmd = cmd;
		get_dir(&data);
		init_minishell(cmd, &data);
		free(data.cmd);
		free(data.my_env);
	}
}
