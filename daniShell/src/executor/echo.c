/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:28 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:05:58 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

static int	is_option(const char *arg, bool *flag)
{
	int	i;

	i = 0;
	if (arg && arg[i] == '-')
		i++;
	else
		return (0);
	while (arg[i] == 'n')
		i++;
	if (arg[i] != '\0')
		return (0);
	*flag = true;
	return (1);
}

int	ft_echo(char **args)
{
	int		i;
	bool	flag;

	i = 0;
	flag = false;
	while (is_option(args[i], &flag) == 1)
		i++;
	while (args[i])
	{
		write(1, args[i], ft_strlen(args[i]));
		if (args[++i])
			write(1, " ", 1);
	}
	if (flag == false)
		write(1, "\n", 1);
	return (0);
}
