/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 14:33:17 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 14:34:17 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <utils.h>

int	check_max_fds(void)
{
	if (data()->open_fds >= MAX_FD)
	{
		print_error_messages("too many fds are open", NULL, WARNING, 0);
		return (1);
	}
	return (0);
}
