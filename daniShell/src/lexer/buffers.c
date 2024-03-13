/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 17:48:22 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/28 17:52:12 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>

t_buffer	*create_buffers(char *input)
{
	t_buffer	*buffers;

	buffers = malloc(sizeof(t_buffer));
	if (!buffers)
		return (NULL);
	buffers->input = ft_strdup(input);
	buffers->buffer = ft_calloc(sizeof(char), (ft_strlen(input) * USHRT_MAX));
	if (!buffers->buffer)
	{
		free((void *)buffers->input);
		free(buffers);
		return (NULL);
	}
	return (buffers);
}

void	clean_buffers(t_buffer *buffers)
{
	free((void *)buffers->input);
	free(buffers->buffer);
	free(buffers);
}

t_buff_ptr	*create_positions(void)
{
	t_buff_ptr	*result;

	result = ft_calloc(1, sizeof(t_buff_ptr));
	if (!result)
		return (NULL);
	return (result);
}

void	clear_positions(t_buff_ptr *positions)
{
	if (positions)
		free(positions);
}
