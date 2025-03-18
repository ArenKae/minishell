/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: keschouf <keschouf@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 09:12:47 by acosi             #+#    #+#             */
/*   Updated: 2023/12/19 09:54:29 by keschouf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	quit_signal(int sig)
{
	if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit: (core dumped)", 2);
		ft_putendl_fd("", 1);
		rl_on_new_line();
	}
}

void	signals_in_exec(void)
{
	signal(SIGINT, in_command_prompt);
	signal(SIGQUIT, quit_signal);
}
