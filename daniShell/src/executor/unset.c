/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 09:44:26 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/09 18:26:19 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

int	ft_unset(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (is_key_in_vars(*env(), args[i]))
		{
			(*env())->delete(*env(), args[i]);
			delete_from_env_vars(args[i], *env());
		}
		i++;
	}
	return (0);
}
