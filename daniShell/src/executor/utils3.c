/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 17:36:43 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 12:58:27 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

char	*get_err_msg(int code, char *arg)
{
	if (!arg)
		return (" command not found");
	if (code == 13)
	{
		if (is_directory(arg))
			return (" Is a directory");
		else if (access(arg, R_OK | X_OK) == -1)
			return (" Permission denied");
	}
	else if (code == 2)
	{
		if ((access(arg, F_OK) == -1
				&& (arg[0] == '/' || arg[0] == '.')) || is_directory(arg))
			return (" No such file or directory");
	}
	return (" command not found");
}

int	get_correct_code(char *arg, int code)
{
	int	status;

	status = -1;
	if (!arg)
		return (127);
	if (code == 2)
	{
		if (access(arg, X_OK) == 0)
			status = 126;
		if (access(arg, F_OK) == -1)
			status = 127;
		if (access(arg, F_OK) == 0 && (*env())->get_value(*env(), "PATH"))
			status = 127;
	}
	else if (code == 13)
	{
		if (is_directory(arg))
			status = 126;
		if (access(arg, X_OK) == -1)
			status = 126;
	}
	return (status);
}

void	put_err_msg(char *arg, char *message)
{
	char	*temp;
	char	*res;

	if (!message)
		message = "unknown error";
	if (!arg)
		arg = "null_argument";
	temp = str_joiner("shitshell: ", arg, 0);
	res = str_joiner(temp, ":", 1);
	temp = str_joiner(res, message, 1);
	ft_putendl_fd(temp, STDERR_FILENO);
	free(temp);
}

void	handle_execve_error(t_command *node, char **arr, int code)
{
	int	exit_code;

	exit_code = 0;
	if (arr[0])
	{
		exit_code = get_correct_code(arr[0], code);
		put_err_msg(arr[0], get_err_msg(code, arr[0]));
	}
	ft_fsplit(arr);
	close_pipeline(node);
	data_destroy();
	exit(exit_code);
}
