/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 11:50:02 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/08 18:54:54 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

int	is_in(char c, char next)
{
	return (c == '<' && next != '<');
}

int	is_out(char c, char next)
{
	return (c == '>' && next != '>');
}

int	is_dquote(char c)
{
	return (c == '\"');
}

int	is_squote(char c)
{
	return (c == '\'');
}

int	redir_lookup(char *str, int pos, int size)
{
	if (!str || size < 0 || pos < 0 || pos + 1 > size)
		return (0);
	if (is_in(str[pos], str[pos + 1]) || is_out(str[pos], str[pos + 1])
		|| is_heredoc(str[pos], str[pos + 1]) || is_append(str[pos],
			str[pos + 1]) || is_pipe(str[pos]))
		return (1);
	return (0);
}
