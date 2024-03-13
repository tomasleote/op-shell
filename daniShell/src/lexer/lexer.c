/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 21:17:34 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 14:30:13 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <utils.h>
#include <lexer.h>
#include <environment.h>

int	lexer(void)
{
	char	*buffer;

	if (!data()->sigint_recived)
		buffer = readline(get_prompt());
	else
	{
		data()->sigint_recived = false;
		return (1);
	}
	if (!buffer)
	{
		free(buffer);
		return (-1);
	}
	if (is_ws_or_empty(buffer, 0))
		return (1);
	lex_input(buffer, &(data()->lexer));
	add_history(data()->last_cmd);
	free(data()->last_cmd);
	data()->last_cmd = NULL;
	return (0);
}
