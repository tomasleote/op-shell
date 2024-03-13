/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/10 17:23:51 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 17:47:57 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>
#include <execution.h>

int	handle_export_errors(char *arg)
{
	char	*temp;
	char	*temp2;

	temp = ft_strjoin("shitshell: export: \'", arg);
	temp2 = str_joiner(temp, "\': not a valid identifier", 1);
	ft_putendl_fd(temp2, STDERR_FILENO);
	free(temp2);
	return (1);
}

int	is_valid_key(char *key)
{
	int	i;

	i = 0;
	if (ft_strlen(key) == 0)
		return (0);
	if (ft_strncmp(key, "__env_vars", 11) == 0
		|| ft_strncmp(key, "__ps1", 6) == 0
		|| ft_strncmp(key, "__tcwd", 7) == 0)
		return (0);
	while (key[i])
	{
		if (i == 0 && ft_isdigit(key[i]))
			return (0);
		if (key[i] != '_' && !ft_isalnum(key[i]))
			return (0);
		i++;
	}
	return (1);
}

void	add_key(char *key, char *value)
{
	char	*temp;

	add_to_env_vars(key);
	if (value != NULL)
	{
		++value;
		if (value)
		{
			temp = ft_strdup(value);
			if (!temp)
				return ;
		}
		if (ft_strlen(key) != 0)
			(*env())->add(*env(), key, temp, 's');
		else
			free(temp);
	}
}
