/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 14:12:22 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 15:42:28 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <utils.h>

int	setup_redirections(t_command **start)
{
	t_command	*temp;
	int			flag;

	flag = 0;
	if (!start || !*start)
		return (0);
	temp = *start;
	while (temp)
	{
		if (temp->token == COMMAND || temp->token == NIL)
		{
			if ((find_prev_redirs(start, temp) < 0)
				|| (find_next_redirs(start, temp) < 0))
			{
				flag = 1;
				break ;
			}
		}
		temp = temp->next;
	}
	if (!can_i_execute(start))
		return (setup_redirections(start));
	if (do_i_join(start))
		join_stuff(start);
	return (flag);
}

int	find_prev_redirs(t_command **start, t_command *command)
{
	t_command	*temp;
	t_command	*temp2;
	int			found;

	found = 0;
	if (!command)
		return (found);
	temp = command;
	while (temp->prev)
		temp = temp->prev;
	while (temp && temp->token != PIPE)
	{
		temp2 = temp->next;
		if (temp == command)
			break ;
		if (is_redir_tok(temp))
		{
			if (create_redir(start, temp, command))
				return (-1);
			found = 1;
		}
		temp = temp2;
	}
	return (found);
}

int	find_next_redirs(t_command **start, t_command *command)
{
	int			found;
	t_command	*temp;
	t_command	*temp2;

	found = 0;
	if (!command)
		return (found);
	temp = command;
	while (temp && temp->token != PIPE)
	{
		temp2 = temp->next;
		if (is_redir_tok(temp))
		{
			if (create_redir(start, temp, command))
				return (-1);
			found = 1;
		}
		temp = temp2;
	}
	return (found);
}

int	can_i_execute(t_command **start)
{
	t_command	*temp;
	int			found;

	found = 0;
	if (!start || !*start)
		return (1);
	temp = *start;
	while (temp)
	{
		if (temp->token == COMMAND || temp->token == NIL)
			found = 1;
		temp = temp->next;
	}
	if (found == 0)
		add_command_back(start, new_command(new_empty_cmd()));
	return (found);
}

void	join_stuff(t_command **start)
{
	t_command	*temp;
	t_command	*temp1;

	if (!*start)
		return ;
	temp = *start;
	while (temp)
	{
		temp1 = temp->next;
		if (temp != *start)
		{
			(*start)->cmd = str_joiner((*start)->cmd, "\x1F", 1);
			(*start)->cmd = str_joiner((*start)->cmd, temp->cmd, 1);
			delete_cmd_node(temp);
		}
		temp = temp1;
	}
}
