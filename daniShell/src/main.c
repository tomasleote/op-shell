/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:10:15 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <lexer.h>
#include <parser.h>
#include <execution.h>

int	main(int ac, char **av, char **envp)
{
	int	status;

	data_init(envp, ac, av);
	while (true)
	{
		setup_signal_handlers();
		status = lexer();
		if (status == 1)
			continue ;
		else if (status == -1)
			break ;
		parser();
		executioner();
	}
	data_destroy();
	return (data()->last_status_code);
}
