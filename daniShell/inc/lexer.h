/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 12:37:49 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/29 10:34:38 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <types.h>

// Main lexing component
int			lexer(void);

//lexing
int			lex_input_loop(t_buffer *buffers, t_buff_ptr *pos);
int			handle_quotes(t_buffer *buffers, t_buff_ptr *pos);
void		lex_input(char *input, t_list **list);
void		set_list(char *buffer, t_list **start);
void		handle_redirections(t_buffer *buffer, t_buff_ptr *pos);

//utils
int			can_quoter_pad_next(t_buffer *buffer, t_buff_ptr *pos);
int			can_quoter_pad_prev(t_buffer *buffer, t_buff_ptr *pos);
void		clean_buffers(t_buffer *buffers);
void		clear_positions(t_buff_ptr *positions);
t_buffer	*create_buffers(char *input);
t_buff_ptr	*create_positions(void);

#endif //LEXER_H
