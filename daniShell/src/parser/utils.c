/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 17:09:40 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 14:29:22 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <minishell.h>

int	do_i_join(t_command **start)
{
	int			counter;
	t_command	*temp;

	if (!*start)
		return (0);
	counter = 0;
	temp = *start;
	while (temp)
	{
		if (temp->token == COMMAND || temp->token == NIL)
			counter++;
		temp = temp->next;
	}
	if (counter > 1)
		return (1);
	else
		return (0);
}

int	is_redir_tok(t_command *cmd)
{
	return (cmd->token == APPEND || cmd->token == HEREDOC
		|| cmd->token == IN || cmd->token == OUT);
}

int	create_redir(t_command **start, t_command *redir, t_command *cmd)
{
	if (redir->token == IN)
	{
		if (set_in(redir, cmd, start))
			return (1);
	}
	else if (redir->token == OUT)
	{
		if (set_out(redir, cmd, start))
			return (1);
	}
	else if (redir->token == APPEND)
	{
		if (set_append(start, redir, cmd))
			return (1);
	}
	else if (redir->token == HEREDOC)
	{
		if (set_heredoc(start, redir, cmd))
			return (1);
	}
	return (0);
}

int	clear_open_fd_memory(char **arr, t_redir *node, t_command *cmd)
{
	if (node->type == IN)
		cmd->redirections.infile = data()->null;
	if (node->type == APPEND || node->type == OUT)
		cmd->redirections.outfile = data()->null;
	perror("shitshell");
	free(node);
	ft_fsplit(arr);
	data()->hidden_code = 1;
	return (1);
}
