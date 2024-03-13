/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:10:42 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <utils.h>

int	count_quotes_pairs(char *str)
{
	int	c;
	int	n_quote;

	c = 0;
	n_quote = 0;
	while (str[c])
	{
		if (is_squote(str[c]))
			n_quote = handle_squote_count(str, &c, n_quote);
		if (is_dquote(str[c]))
			n_quote = handle_dquote_count(str, &c, n_quote);
		c++;
	}
	return (n_quote);
}

t_quote	*init_quotes(int n_quote)
{
	int		c;
	t_quote	*quotes;

	c = 0;
	if (!n_quote || (n_quote % 2 != 0))
		return (NULL);
	quotes = malloc(((n_quote / 2) + 1) * sizeof(t_quote));
	if (!quotes)
		return (NULL);
	while (c < (n_quote / 2) + 1)
	{
		quotes[c].start = -1;
		quotes[c].end = -1;
		c++;
	}
	return (quotes);
}

t_quote	*fill_quotes(char *str, t_quote *quotes)
{
	int	c;
	int	i;

	c = 0;
	i = 0;
	while (str[c])
	{
		if (is_squote(str[c]))
		{
			quotes[i].start = c++;
			while (!is_squote(str[c]))
				c++;
			quotes[i++].end = c;
		}
		else if (is_dquote(str[c]))
		{
			quotes[i].start = c++;
			while (!is_dquote(str[c]))
				c++;
			quotes[i++].end = c;
		}
		c++;
	}
	return (quotes);
}

int	is_to_remove(int index, t_quote *quotes)
{
	int	i;

	i = 0;
	while (quotes[i].end && quotes[i].end != -1)
	{
		if (quotes[i].start == index || quotes[i].end == index)
			return (1);
		i++;
	}
	return (0);
}

char	*remove_quotes(char *str, t_quote *quotes)
{
	int		c;
	int		i;
	char	*new_str;

	c = 0;
	i = 0;
	new_str = ft_calloc(sizeof(char), ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	while (str[c])
	{
		if (!is_to_remove(c, quotes))
			new_str[i++] = str[c];
		c++;
	}
	free(str);
	return (new_str);
}
