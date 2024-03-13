/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:06:23 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

int	ft_export(char **args)
{
	if (args && !args[1])
	{
		print_export();
		return (0);
	}
	return (add_args_if_possible(args));
}

void	print_export(void)
{
	int		i;
	char	**temp;
	char	*value;

	i = 0;
	temp = get_env_keys(*env());
	while (i < data()->env_size)
	{
		printf("declare -x %s", temp[i]);
		value = (*env())->get_value(*env(), temp[i]);
		if (value)
			printf("=\"%s\"", value);
		printf("\n");
		i++;
	}
	ft_fsplit(temp);
}

int	add_args_if_possible(char **args)
{
	int		i;
	int		signal;

	i = 1;
	signal = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "=", 2) == 0)
		{
			signal = handle_export_errors(args[i++]);
			continue ;
		}
		signal = add_to_environment(args[i]);
		i++;
	}
	return (signal);
}

int	add_to_environment(char *arg)
{
	char	*key;
	char	*value;
	int		signal;

	signal = 0;
	value = ft_strnstr(arg, "=", ft_strlen(arg));
	key = extract_key(arg);
	if (key)
	{
		if (is_valid_key(key))
			add_key(key, value);
		else
			signal = handle_export_errors(arg);
		free(key);
	}
	return (signal);
}
