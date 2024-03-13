/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 12:23:19 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 14:30:02 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

void	executioner(void)
{
	if (is_valid_list(&(data()->commands))
		&& (data()->exited_on_heredoc == false))
		execute_list(data()->commands);
	update_env();
	if (data()->commands)
	{
		clear_cmd_lst(data()->commands);
		data()->commands = NULL;
	}
	data()->open_fds = 1;
	data()->hidden_code = 0;
	data()->is_executing = 0;
}

void	fucking_cat(t_command *node)
{
	if ((node->redirections.infile == -1 && node->redirections.outfile != -1) \
	|| (node->redirections.infile == -1 && node->redirections.outfile == -1))
		node->redirections.infile = dup(data()->null);
	if (node->redirections.outfile == -1 && node->redirections.infile == -1)
		node->redirections.outfile = dup(data()->null);
}

int	is_valid_list(t_command **start)
{
	t_command	*temp;
	t_command	*temp1;

	if (!start || !*start)
		return (0);
	temp = *start;
	while (temp)
	{
		temp1 = temp->next;
		if ((!temp->cmd && !temp->cmd_path && temp->token == NIL))
		{
			if (temp->next != NULL)
				fucking_cat(temp->next);
			remove_node_cmdlst(start, temp);
		}
		temp = temp1;
	}
	return (1);
}

void	execute_list(t_command *start)
{
	t_command	*tmp;

	if (!start)
		return ;
	tmp = start;
	data()->is_executing = 1;
	if (tmp->next)
		open_pipeline(start);
	while (tmp)
	{
		if (data()->hidden_code && !tmp->prev && !tmp->next)
			break ;
		if (tmp->token == COMMAND)
			execute_token_is_command(tmp);
		else if (tmp->token == NIL)
			execute_token_is_nil(tmp);
		tmp = tmp->next;
	}
	wait_processes(start);
	close_pipeline(start);
}
