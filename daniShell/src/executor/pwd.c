/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danalmei <danalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:54:29 by danalmei          #+#    #+#             */
/*   Updated: 2024/02/29 11:09:51 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

int	ft_pwd(char **args)
{
	char	*temp;

	(void)args;
	temp = (*env())->get_value(*env(), "PWD");
	if (!temp)
	{
		add_to_env_vars("PWD");
		temp = getcwd(NULL, 0);
		if (!temp)
			perror("shitshell");
		else
		{
			printf("%s\n", temp);
			(*env())->add(*env(), "PWD", temp, 's');
		}
		return (0);
	}
	printf("%s\n", temp);
	return (0);
}
