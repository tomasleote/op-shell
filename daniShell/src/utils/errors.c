/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 13:45:56 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/29 22:53:09 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

static char	*get_type_message(t_error type)
{
	if (type == SYNTAX)
		return ("syntax error: ");
	if (type == CD)
		return ("cd: ");
	if (type == ENV)
		return ("env: ");
	if (type == EXIT)
		return ("exit: ");
	if (type == WARNING)
		return ("warning: ");
	return ("undefined error: ");
}

char	*create_custom_message(char *message, char *argument)
{
	char	*result;
	char	*temp;

	result = str_joiner(message, "\"", 0);
	temp = str_joiner(result, argument, 1);
	result = str_joiner(temp, "\"", 1);
	return (result);
}

void	print_error_messages(char *message, char *argument, t_error type,
		int flag)
{
	char	*base_msg;
	char	*temp1;
	char	*temp;
	int		signal;

	if (flag)
		signal = 3;
	else
		signal = 1;
	base_msg = ft_strdup("shitshell: ");
	temp1 = str_joiner(base_msg, get_type_message(type), 1);
	if (argument)
	{
		base_msg = create_custom_message(message, argument);
		temp = str_joiner(temp1, base_msg, 3);
		base_msg = temp;
		if (flag)
			free(message);
	}
	else
		base_msg = str_joiner(temp1, message, signal);
	ft_putendl_fd(base_msg, STDERR_FILENO);
	free(base_msg);
}
