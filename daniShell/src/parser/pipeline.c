/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 22:37:31 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 16:05:25 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <utils.h>

void	setup_outputs_and_inputs(t_command **start)
{
	t_command	*temp1;
	t_command	*temp2;

	if (!start || !*start)
		return ;
	temp1 = *start;
	while (temp1)
	{
		temp2 = temp1->next;
		if (temp1->token == NIL || temp1->token == COMMAND)
		{
			set_inputs_and_outs(temp1);
			dup_ins_outs(temp1);
			clear_redir_lst(temp1->redirs);
			temp1->redirs = NULL;
		}
		else
			remove_node_cmdlst(start, temp1);
		temp1 = temp2;
	}
}

void	join_pipeline(t_command **array, int size, t_command **start)
{
	int			i;
	t_command	*temp;

	i = 0;
	if (!start)
		return ;
	while (i < size)
	{
		temp = array[i];
		while (temp)
		{
			move_node_to_lst(start, temp);
			temp = temp->next;
		}
		i++;
	}
}

void	setup_pipes(t_command **start)
{
	t_command	*temp;

	if (!start || !*start)
		return ;
	temp = *start;
	if (cmd_lst_size(temp) == 1)
		return ;
	while (temp)
	{
		temp->pipe = true;
		temp = temp->next;
	}
}
