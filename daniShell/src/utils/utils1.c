/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 13:22:42 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/03 19:28:32 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>
#include <minishell.h>

int	is_ws_or_empty(char *str, int flag)
{
	int	i;

	i = 0;
	if (ft_strlen(str) == 0)
	{
		free(str);
		return (1);
	}
	while (str[i])
		if (!is_whitespace(str[i++]))
			return (0);
	if (!flag)
	{
		add_history(str);
		free(str);
	}
	return (1);
}

int	is_dollar(char c)
{
	return (c == '$');
}

int	is_builtin(char *str)
{
	if ((ft_strlen(str) == 3 && ft_strncmp(str, "env", 3) == 0)
		|| (ft_strlen(str) == 2 && ft_strncmp(str, "cd", 2) == 0)
		|| (ft_strlen(str) == 3 && ft_strncmp(str, "pwd", 3) == 0)
		|| (ft_strlen(str) == 4 && ft_strncmp(str, "echo", 4) == 0)
		|| (ft_strlen(str) == 4 && ft_strncmp(str, "exit", 4) == 0)
		|| (ft_strlen(str) == 5 && ft_strncmp(str, "unset", 5) == 0)
		|| (ft_strlen(str) == 6 && ft_strncmp(str, "export", 6) == 0))
		return (1);
	return (0);
}

char	*extract_key(const char *arg)
{
	int		i;
	int		k;
	char	*key;

	i = 0;
	k = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	if (i == 0)
		return (NULL);
	key = malloc(i + 1);
	if (!key)
		return (NULL);
	while (i != k)
	{
		key[k] = arg[k];
		k++;
	}
	key[k] = '\0';
	return (key);
}

char	*ft_strstr(char *haystack, char *needle)
{
	char	*res;

	if (!haystack || !needle)
		return (NULL);
	res = ft_strnstr(haystack, needle, ft_strlen(needle));
	if (res)
		return (res + ft_strlen(needle));
	else
		return (NULL);
}
