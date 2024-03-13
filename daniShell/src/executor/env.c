/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 14:46:56 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/01 12:15:01 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <utils.h>

int	ft_env(char **args)
{
	int		i;
	char	**envp;

	if (args[1])
	{
		print_error_messages("too many arguments", NULL, ENV, 0);
		return (1);
	}
	i = 0;
	envp = data()->envp;
	while (i < data()->env_size)
	{
		if (envp[i] != NULL)
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
