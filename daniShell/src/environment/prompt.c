/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 23:03:17 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/07 16:50:43 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environment.h>
#include <utils.h>

void	set_prompt(char *user)
{
	char	*res;
	char	*temp;
	char	*cwd;
	char	*host;

	(*env())->add(*env(), "__tcwd", get_trimmed_cwd(), 's');
	if (!user)
		(*env())->add(*env(), "__ps1", ft_strdup("Bitch give me env -> "),
			's');
	else
	{
		host = (*env())->get_value(*env(), "HOST");
		cwd = (*env())->get_value(*env(), "__tcwd");
		temp = str_joiner(user, "@", 0);
		res = str_joiner(temp, host, 1);
		temp = str_joiner(res, " ", 1);
		res = str_joiner(temp, cwd, 1);
		temp = str_joiner(res, "$ ", 1);
		(*env())->add(*env(), "__ps1", temp, 's');
	}
}

const char	*get_prompt(void)
{
	const char	*ps1;

	set_prompt((*env())->get_value(*env(), "USER"));
	ps1 = (const char *)(*env())->get_value(*env(), "__ps1");
	return (ps1);
}
