/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acosi <acosi@student.42nice.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 13:30:59 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 09:15:49 by acosi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Handler function in case of SIGINT (Ctrl + C).
    rl_replace_line() flush the current line and replace it with an empty one.
    ft_putendl_fd() is used to put a newline character.
    rl_on_new_line() notify readline that we want to start a new line.
    rl_redisplay() refresh the display with the content of the last buffer.
	Additionnaly, we have to check the status of the g_signal global 
	variable because if a heredoc was not exited properly, the readline function 
	puts an additionnal space (might be a bug).
*/

void	flush_prompt(int sig)
{
	if (sig == SIGINT && g_signal < 2)
	{
		write(STDERR_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_signal = CTRL_C;
	}
	if (sig == SIGINT && g_signal >= 2)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

/*
	Sentinel function to catch some signals. SIGINT (Ctrl-c) is sent 
	to the flush_prompt() function. SIGQUIT (Ctrl-\) is ignored.
*/

void	signals(void)
{
	signal(SIGINT, flush_prompt);
	signal(SIGQUIT, SIG_IGN);
}

//	Interrupt the prompt and add a new line in case of SIGINT.

void	in_command_prompt(int sig)
{
	if (sig == SIGINT)
	{
		ft_putendl_fd("", 1);
		rl_on_new_line();
	}
}

/*
	Interrupt the prompt and add a new line in case of SIGINT inside a heredoc.
	Additionaly, the standard input needs to be closed to bypass the while(1) 
	loop inside the heredoc. The STDIN will be reformed later.
*/

void	in_heredoc_prompt(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = CTRL_C_HEREDOC;
		rl_replace_line("", 1);
		ft_putendl_fd("", 1);
		close(STDIN_FILENO);
	}
}

/*
	Special case signal handler when a new instance of minishell is launched 
	inside the minishell. This allows us to avoid confusing signal handling
	on multiple levels.
*/

void	miniception_signals(int sig)
{
	if (sig != SIGINT)
		return ;
}
