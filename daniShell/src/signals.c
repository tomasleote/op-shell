/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danalmei <danalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 23:37:09 by danalmei          #+#    #+#             */
/*   Updated: 2024/03/10 17:11:46 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <parser.h>

void	reset_prompt(void)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler(int signo)
{
	(void)signo;
	data()->sigint_recived = true;
	if (data()->is_executing)
		printf("\n");
	else if (data()->on_heredoc)
		data()->exited_on_heredoc = true;
	else if (!data()->commands)
	{
		printf("\n");
		data()->last_status_code = 130;
		reset_prompt();
	}
}

void	sig_handle_doc(int signo)
{
	(void)signo;
	pipeline_destroy();
	data_destroy();
	exit(130);
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data()->on_heredoc = false;
	data()->exited_on_heredoc = false;
	data()->is_executing = false;
	data()->sigint_recived = false;
}
