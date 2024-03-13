/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:41:59 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/09 22:28:23 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environment.h>

int	is_key_in_vars(t_hash *table, char *key)
{
	int		i;
	char	*_env_keys;
	char	**keys;

	if (!table || !key)
		return (0);
	i = 0;
	_env_keys = table->get_value(table, "__env_vars");
	keys = ft_split(_env_keys, '$');
	while (keys && keys[i])
	{
		if (ft_strncmp(keys[i], key, ft_strlen(key)) == 0)
			return (ft_fsplit(keys), 1);
		i++;
	}
	ft_fsplit(keys);
	return (0);
}

static void	sort_result(char **array)
{
	int		i;
	int		k;
	char	*temp;

	i = 1;
	while (array[i])
	{
		temp = array[i];
		k = i - 1;
		while (k >= 0 && array[k][0] > temp[0])
		{
			array[k + 1] = array[k];
			k = k - 1;
		}
		array[k + 1] = temp;
		i++;
	}
}

char	**get_env_keys(t_hash *table)
{
	char	**result;
	char	*temp;

	temp = table->get_value(table, "__env_vars");
	if (temp)
	{
		result = ft_split(temp, '$');
		if (result)
		{
			sort_result(result);
			return (result);
		}
		return (NULL);
	}
	return (NULL);
}

void	reset_env_vars(t_hash *table, char **keys)
{
	int	i;

	i = 0;
	table->add(table, "__env_vars", ft_strdup(""), 's');
	while (keys[i])
		add_to_env_vars(keys[i++]);
}

int	delete_from_env_vars(char *key, t_hash *table)
{
	char	*keys;
	char	**key_arr;
	int		i;
	int		k;

	keys = (char *)table->get_value(table, "__env_vars");
	if (!keys || !is_key_in_vars(table, key))
		return (0);
	key_arr = ft_split(keys, '$');
	i = -1;
	while (key_arr[++i])
		if (ft_strncmp(key_arr[i], key, ft_strlen(key)) == 0)
			break ;
	if (!key_arr[i])
		return (ft_fsplit(key_arr), 0);
	free(key_arr[i]);
	k = i + 1;
	while (key_arr[k])
		key_arr[i++] = key_arr[k++];
	key_arr[i] = NULL;
	reset_env_vars(table, key_arr);
	ft_fsplit(key_arr);
	return (1);
}
