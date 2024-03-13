/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 18:34:08 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/07 14:16:23 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environment.h>
#include <utils.h>

void	set_shell_level(t_hash *table, char *value)
{
	long	current_level;

	if (!value)
	{
		table->add(table, "SHLVL", ft_itoa(1), 's');
		add_to_env_vars(ft_strdup("$SHLVL"));
	}
	else
	{
		current_level = ft_atol(value);
		if (current_level <= 0 || current_level > INT_MAX)
			current_level = -1;
		table->add(table, "SHLVL", ft_itoa((int)(current_level + 1)), 's');
	}
}

char	*get_hostname(void)
{
	int		hostfd;
	char	*res;
	char	*line;

	hostfd = open("/etc/hostname", O_RDONLY);
	line = get_next_line(hostfd);
	res = ft_strclean(line, '\n', ft_strlen(line));
	free(line);
	close(hostfd);
	return (res);
}

char	*get_trimmed_cwd(void)
{
	char	*cwd;
	char	*home;
	char	*res;

	cwd = getcwd(NULL, 0);
	home = (*env())->get_value(*env(), "HOME");
	if (cwd)
	{
		res = ft_strstr(cwd, home);
		if (!res)
			return (cwd);
		res = str_joiner("~", res, 0);
		free(cwd);
		return (res);
	}
	return (ft_strdup("bro stop trying to break me"));
}

int	add_to_env_vars(char *key)
{
	char	*keys;
	char	*temp;
	char	*temp1;
	t_hash	*table;

	table = *env();
	keys = (char *)table->get_value(table, "__env_vars");
	if (!keys || !key || is_key_in_vars(table, key))
		return (0);
	temp = str_joiner("$", key, 0);
	temp1 = str_joiner(keys, temp, 2);
	table->add(table, "__env_vars", temp1, 's');
	return (1);
}

char	**create_env(int size, char **keys)
{
	char	**result;
	char	*value;
	char	*temp1;
	int		i;

	i = 0;
	result = ft_calloc((size + 1), sizeof(char *));
	if (!result)
		return (NULL);
	while (keys[i])
	{
		value = (char *)(*env())->get_value(*env(), keys[i]);
		if (value)
		{
			temp1 = ft_strjoin(keys[i], "=");
			result[i] = ft_strjoin(temp1, value);
			free(temp1);
		}
		i++;
	}
	result[i] = 0;
	return (result);
}
