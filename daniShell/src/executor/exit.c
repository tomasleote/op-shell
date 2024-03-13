/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:14:49 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/06 23:59:45 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

int	ft_exit(char **args, t_command *node)
{
	int		count;
	int		code;
	long	temp;
	char	**tmp_args;

	tmp_args = (args + 1);
	ft_putendl_fd("exit", STDERR_FILENO);
	count = count_args(tmp_args);
	if (count > 1)
	{
		print_error_messages("too many arguments", NULL, EXIT, 0);
		return (1);
	}
	else if (count == 0)
		ft_quit(args, 0, NULL, node);
	temp = ft_atol(tmp_args[0]);
	if (temp < INT_MIN || temp > INT_MAX || is_string(tmp_args[0]))
		ft_quit(args, 2, "shitshell: exit: numeric argument required", node);
	code = (int)temp % 256;
	return (ft_quit(args, code, NULL, node));
}

int	ft_quit(char **args, int code, char *message, t_command *node)
{
	(void)args;
	if (message)
		ft_putendl_fd(message, STDERR_FILENO);
	if (args)
		ft_fsplit(args);
	close_fds(node);
	close_pipeline(node);
	data_destroy();
	close(0);
	close(1);
	exit(code);
}
