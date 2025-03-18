/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 11:18:45 by keschouf          #+#    #+#             */
/*   Updated: 2023/12/19 09:53:20 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	solo_exec_wait_pid(pid_t pid, t_data *data)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		status = WEXITSTATUS(status);
	else
	{
		status = WTERMSIG(status);
		status += 128;
	}
	data->status = status;
}

/*
	This function handles the execution of a single command.
	First the signals are handled differently in case a new instance of minishell
	is opened inside minishell.
	Then, a new process is created to execute the command after checking for
	redirections.
*/

void	solo_exec(t_chain **itr, t_struct **head, t_data *data, char *cmd)
{
	pid_t		pid;
	int			fd;
	int			pipe_hdc[2];
	t_redirect	redirect;
	t_chain		*hdc;

	if ((*itr)->content && (ft_strnstr((*itr)->content, "./minishell\0", 13)))
		signal(SIGINT, miniception_signals);
	else
		signals_in_exec();
	pid = fork();
	if (pid == 0)
	{
		hdc = check_redirect_init(*itr, &redirect, 0);
		make_exec_redirect(&fd, redirect);
		if (hdc)
			heredoc_redir(pipe_hdc, hdc, data, cmd);
		if (redirect.error == 1)
			ft_exit_solo(data, *itr, head);
		exec_init(*itr, itr, head, data);
	}
	solo_exec_wait_pid(pid, data);
}

/*
	Initializes the execution of non-forked builtins.
	First, some additional structures are created and filled for handling 
	heredocs and redirections, as well as checking for errors.
	If a redirection flag was found, the corresponding file is opened.
	Else, if a heredoc was found, it will be redirected inside a pipe as 
	a separate process. Finally, if no redirection was done, we enter the 
	exec_init() function, which handles execution of the command or builtin.
*/

void	solo_builtin_exec_init(t_chain **itr, t_struct **head,
	t_data *data, char *cmd)
{
	int			fd;
	int			pipe_hdc[2];
	t_redirect	redirect;
	t_chain		*hdc;

	(void)fd;
	signal(SIGINT, in_command_prompt);
	hdc = check_redirect_init(*itr, &redirect, 0);
	if (redirect.out_file && redirect.is_append != 1)
		fd = open(redirect.out_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redirect.out_file)
		fd = open(redirect.out_file, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if (hdc)
		heredoc_redir(pipe_hdc, hdc, data, cmd);
	if (redirect.error == 1)
	{
		data->status = 1;
		return ;
	}
	else
		exec_init(*itr, itr, head, data);
}

/*
	If no pipes are found during parsing, this function is responsible for 
	executing independant commands. First, it will try to identify any builtins.
	Depending on the result, some builtins need to be handled separately because 
	they can't be forked (export, unset, cd and exit). Those builtins are 
	initialized by their own function to ensure they are executed by the main
	calling process of the minishell. The others as well as non-builtins 
	commands are forked inside the solo_exec function.
*/

void	solo_exec_init(t_chain **itr, t_struct **head, t_data *data, char *cmd)
{
	if ((*itr)->content == NULL)
	{
		while ((*itr)->next)
		{
			(*itr) = (*itr)->next;
			if ((*itr)->content)
				break ;
		}
	}
	if (is_builtin((*itr)->content) < 4)
		solo_exec(itr, head, data, cmd);
	else if (is_builtin((*itr)->content))
		solo_builtin_exec_init(itr, head, data, cmd);
	else
		solo_exec(itr, head, data, cmd);
}
