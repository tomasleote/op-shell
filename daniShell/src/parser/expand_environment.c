/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_environment.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:08:00 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environment.h>
#include <parser.h>
#include <utils.h>

char	*expand_env_var(char *str, t_quote *quotes)
{
	int		c;
	char	*new_str;

	c = 0;
	new_str = (char *)ft_calloc(sizeof(char), 1);
	if (!new_str)
		return (NULL);
	new_str[0] = '\0';
	while (str[c])
	{
		if (quotes)
			new_str = skip_squote(str, new_str, quotes, &c);
		while (str[c] == '$' && str[c + 1])
		{
			new_str = swap_env_key(&str[c + 1], new_str, &c);
			c++;
		}
		if (str[c])
			new_str = add_char_to_str(new_str, str[c++]);
	}
	free(str);
	return (new_str);
}

char	*skip_squote(char *str, char *new_str, t_quote *quotes, int *c)
{
	int	i;

	i = 0;
	while (quotes[i].end && quotes[i].end != -1)
	{
		if ((*c >= quotes[i].start && *c < quotes[i].end)
			&& is_squote(str[quotes[i].start]))
		{
			while (*c <= quotes[i].end)
			{
				new_str = (char *)add_char_to_str(new_str, str[*c]);
				if (str[*c])
					*c += 1;
			}
			return (new_str);
		}
		i++;
	}
	return (new_str);
}

char	*swap_env_key(char *str, char *new_str, int *c)
{
	char	*key;
	int		key_len;
	char	*value;

	key = extract_env_key(str);
	if (!key && (str[0] == '?' || str[0] == '$'))
		return (special_expansions(str, new_str, c));
	key_len = (int)ft_strlen(key);
	if (key_len == 0)
	{
		new_str = add_char_to_str(new_str, '$');
		return (new_str);
	}
	*c += key_len;
	if (key)
	{
		value = (*env())->get_value(*env(), key);
		if (value)
			new_str = add_wrd_to_str(new_str, value);
		free(key);
	}
	return (new_str);
}

char	*extract_env_key(char *start)
{
	int		c;
	char	*key;

	c = 0;
	while (start[c])
	{
		if (c == 0 && ft_isdigit(start[c]))
		{
			c++;
			break ;
		}
		if (!ft_isalnum(start[c]) && start[c] != '_')
			break ;
		c++;
	}
	if (!c)
		return (NULL);
	key = (char *)ft_calloc(sizeof(char), c + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, start, (c + 1));
	key[c] = '\0';
	return (key);
}

char	*expand_env_var_heredoc(char *str)
{
	int		c;
	char	*new_str;

	c = 0;
	new_str = (char *)ft_calloc(sizeof(char), 1);
	if (!new_str)
		return (NULL);
	new_str[0] = '\0';
	while (str[c])
	{
		while (str[c] == '$' && str[c + 1])
		{
			new_str = swap_env_key(&str[c + 1], new_str, &c);
			c++;
		}
		if (str[c])
			new_str = add_char_to_str(new_str, str[c++]);
	}
	free(str);
	return (new_str);
}
