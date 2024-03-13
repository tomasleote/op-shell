/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 15:29:31 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/16 16:38:56 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <utils.h>

int	can_quoter_pad_next(t_buffer *buffer, t_buff_ptr *pos)
{
	int	flag;

	flag = 1;
	if (pos->i < (int)(ft_strlen(buffer->input))
			&& buffer->input[pos->i]
			&& (is_quote(buffer->input[pos->i])
			|| !is_whitespace(buffer->input[pos->i]))
			)
		flag = 0;
	return (flag);
}

int	can_quoter_pad_prev(t_buffer *buffer, t_buff_ptr *pos)
{
	int	flag;

	flag = 1;
	if (pos->i - 1 > 0
		&& buffer->input[pos->i - 1] && (is_quote(buffer->input[pos->i - 1])
			|| !is_whitespace(buffer->input[pos->i - 1])))
		flag = 0;
	return (flag);
}
