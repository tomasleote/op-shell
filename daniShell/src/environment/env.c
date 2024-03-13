/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 19:48:04 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/09 23:26:54 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <environment.h>
#include <utils.h>

t_hash	**env(void)
{
	static t_hash	*env;

	return (&env);
}

void	set_env(char **envp)
{
	int		i;
	char	*key;
	char	*value;

	set_minimum_env();
	if (!envp)
		return ;
	i = -1;
	while (envp[++i] != 0)
	{
		value = ft_strnstr(envp[i], "=", ft_strlen(envp[i]));
		key = extract_key(envp[i]);
		if (key)
		{
			add_to_env_vars(key);
			if (value++)
			{
				if (value)
					(*env())->add(*env(), key, ft_strdup(value), 's');
			}
			free(key);
		}
	}
	(*env())->add(*env(), "SHELL", ft_strdup("shitshell"), 's');
}

void	set_minimum_env(void)
{
	(*env())->add(*env(), "HOST", get_hostname(), 's');
	(*env())->add(*env(), "PWD", getcwd(NULL, 0), 's');
	(*env())->add(*env(), "OLDPWD", NULL, 's');
	(*env())->add(*env(), "SHELL", ft_strdup("shitshell"), 's');
	add_to_env_vars("SHELL");
	add_to_env_vars("PWD");
	add_to_env_vars("OLDPWD");
}

char	**get_env(t_hash *table)
{
	char	*vars;
	char	**keys;
	char	**result;
	int		size;

	size = 0;
	vars = (char *)table->get_value(table, "__env_vars");
	keys = ft_split(vars, '$');
	while (keys[size])
		size++;
	result = create_env(size, keys);
	data()->env_size = size;
	ft_fsplit(keys);
	return (result);
}

void	update_env(void)
{
	int		i;
	t_hash	*table;

	table = *env();
	i = 0;
	if (data()->envp)
	{
		while (i < data()->env_size)
			free(data()->envp[i++]);
		free(data()->envp);
		data()->envp = NULL;
	}
	data()->envp = get_env(table);
}
