/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 20:04:16 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:07:37 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <minishell.h>
#include <utils.h>

void	lex_input(char *input, t_list **list)
{
	t_buffer	*buffers;
	t_buff_ptr	*positions;

	buffers = create_buffers(input);
	positions = create_positions();
	free(input);
	if (lex_input_loop(buffers, positions))
	{
		data()->last_status_code = 1;
		data()->last_cmd = ft_strdup(buffers->input);
		print_error_messages("unclosed quotes", NULL, SYNTAX, 0);
		clean_buffers(buffers);
		clear_positions(positions);
		return ;
	}
	data()->last_cmd = ft_strdup(buffers->input);
	set_list(buffers->buffer, list);
	clear_positions(positions);
	clean_buffers(buffers);
}

int	lex_input_loop(t_buffer *buffers, t_buff_ptr *pos)
{
	while (pos->i < (int)ft_strlen(buffers->input))
	{
		if (redir_lookup((char *)buffers->input, pos->i,
				(int)ft_strlen(buffers->input)))
			handle_redirections(buffers, pos);
		else if (is_quote(buffers->input[pos->i]))
		{
			if (handle_quotes(buffers, pos))
				return (1);
		}
		else if (is_whitespace(buffers->input[pos->i]))
		{
			buffers->buffer[pos->k++] = 0x1B;
			pos->i++;
		}
		else
			buffers->buffer[pos->k++] = buffers->input[pos->i++];
	}
	return (0);
}

void	set_list(char *buffer, t_list **start)
{
	int		i;
	t_list	**ptr;
	char	**temp;

	i = -1;
	ptr = (t_list **)start;
	temp = ft_split(buffer, 27);
	while (temp[++i])
		ft_lstadd_back(ptr, ft_lstnew((void *)ft_strdup(temp[i])));
	ft_fsplit(temp);
}

void	handle_redirections(t_buffer *buffer, t_buff_ptr *pos)
{
	if (is_heredoc(buffer->input[pos->i], buffer->input[pos->i + 1])
		|| is_append(buffer->input[pos->i], buffer->input[pos->i + 1]))
	{
		buffer->buffer[pos->k] = 0x1B;
		buffer->buffer[pos->k + 1] = buffer->input[pos->i];
		buffer->buffer[pos->k + 2] = buffer->input[pos->i];
		buffer->buffer[pos->k + 3] = 0x1B;
		pos->k += 4;
		pos->i += 2;
	}
	else
	{
		buffer->buffer[pos->k] = 0x1B;
		buffer->buffer[pos->k + 1] = buffer->input[pos->i++];
		buffer->buffer[pos->k + 2] = 0x1B;
		pos->k += 3;
	}
}

int	handle_quotes(t_buffer *buffers, t_buff_ptr *pos)
{
	t_quote	quote;

	if (can_quoter_pad_prev(buffers, pos))
		buffers->buffer[pos->k++] = 0x1B;
	quote = (t_quote){buffers->input[pos->i], pos->i, -1};
	buffers->buffer[pos->k++] = buffers->input[pos->i++];
	while (buffers->input[pos->i])
	{
		if (buffers->input[pos->i] && buffers->input[pos->i] == quote.quote)
			break ;
		buffers->buffer[pos->k++] = buffers->input[pos->i++];
	}
	if (buffers->input[pos->i] != '\0')
		buffers->buffer[pos->k++] = buffers->input[pos->i++];
	else
		return (1);
	if (can_quoter_pad_next(buffers, pos))
		buffers->buffer[pos->k++] = 0x1B;
	return (0);
}
