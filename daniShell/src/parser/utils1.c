/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 16:40:55 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 16:05:35 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>
#include <minishell.h>
#include <parser.h>

char	*add_char_to_str(char *str, char ch)
{
	int		new_len;
	char	*new_str;

	if (!ch || !str)
		return (str);
	new_len = (int)ft_strlen(str) + 2;
	new_str = malloc(sizeof(char) * new_len);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, new_len);
	new_str[new_len - 2] = ch;
	new_str[new_len - 1] = '\0';
	free(str);
	return (new_str);
}

char	*add_wrd_to_str(char *str, char *wrd)
{
	char	*tmp_str;

	if (!wrd || !str)
		return (str);
	while (*wrd)
	{
		tmp_str = add_char_to_str(str, *wrd);
		if (!tmp_str)
		{
			free(str);
			return (NULL);
		}
		str = tmp_str;
		wrd++;
	}
	return (str);
}

char	*special_expansions(char *str, char *new_str, int *c)
{
	char	*last_status;

	last_status = ft_itoa(data()->last_status_code);
	if (str[0] == '?')
	{
		new_str = add_wrd_to_str(new_str, last_status);
		*c += 1;
		free(last_status);
		return (new_str);
	}
	else
	{
		free(last_status);
		return (add_wrd_to_str(new_str, ""));
	}
}

int	handle_squote_count(char *str, int *i, int nquote)
{
	int	k;

	k = *i;
	nquote++;
	while (!is_squote(str[k]))
		k++;
	if (str[k] && is_squote(str[k]))
		nquote++;
	*i = k;
	return (nquote);
}

int	handle_dquote_count(char *str, int *i, int nquote)
{
	int	k;

	k = *i;
	nquote++;
	while (!is_dquote(str[k]))
		k++;
	if (str[k] && is_dquote(str[k]))
		nquote++;
	*i = k;
	return (nquote);
}
