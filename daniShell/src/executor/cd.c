/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danalmei <danalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 14:25:33 by danalmei          #+#    #+#             */
/*   Updated: 2024/03/09 18:23:49 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <utils.h>

int	ft_cd(char **args)
{
	int		signal;
	char	*cwd;

	cwd = (*env())->get_value(*env(), "PWD");
	signal = check_cd_args(args, cwd);
	if (signal == 0)
	{
		if (args[1] && ft_access(args[1]))
			return (1);
		if (cwd)
			(*env())->add(*env(), "OLDPWD", ft_strdup(cwd), 's');
		chdir(args[1]);
		(*env())->add(*env(), "PWD", getcwd(NULL, 0), 's');
		return (0);
	}
	else if (signal == -1)
		return (0);
	else
		return (1);
}

int	check_cd_args(char **args, char *cwd)
{
	char	*temp;
	char	*temp1;
	char	*oldpwd;

	oldpwd = (*env())->get_value(*env(), "OLDPWD");
	if (!args || !args[1])
		return (going_home((*env())->get_value(*env(), "PWD"), cwd));
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		temp = args[1];
		if (oldpwd)
		{
			temp1 = ft_strdup(oldpwd);
			args[1] = temp1;
			free(temp);
		}
	}
	if (args[2])
	{
		print_error_messages("too many arguments", NULL, CD, 0);
		return (1);
	}
	return (0);
}

int	going_home(char *pwd, char *cwd)
{
	char	*home;

	if (!pwd)
		return (1);
	home = (*env())->get_value(*env(), "HOME");
	if (!home)
	{
		print_error_messages("HOME not set", NULL, CD, 0);
		return (1);
	}
	if (cwd)
		(*env())->add(*env(), "OLDPWD", ft_strdup(cwd), 's');
	chdir(home);
	(*env())->add(*env(), "PWD", getcwd(NULL, 0), 's');
	return (-1);
}
