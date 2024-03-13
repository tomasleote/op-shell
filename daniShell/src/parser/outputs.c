/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   outputs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 22:03:28 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 10:55:38 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>
#include <parser.h>
#include <minishell.h>

int	set_out(t_command *redir, t_command *cmd, t_command **start)
{
	char	**temp;
	t_redir	*new;

	temp = ft_split(redir->cmd, 0x1F);
	if (!temp[1] || ft_strlen(temp[1]) == 0)
		return (print_error_messages("invalid redirection token",
				NULL, SYNTAX, 0), ft_fsplit(temp), 1);
	if (data()->open_fds < MAX_FD)
	{
		new = new_redir(new_base_redir(redir));
		new->fds.outfile = open(temp[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (new->fds.outfile < 0)
			return (clear_open_fd_memory(temp, new, cmd));
		data()->open_fds++;
		ft_fsplit(temp);
		add_redir_back(&(cmd->redirs), new);
		remove_node_cmdlst(start, redir);
		return (0);
	}
	ft_fsplit(temp);
	print_error_messages("too many fds are open", NULL, WARNING, 0);
	return (1);
}

int	set_append(t_command **start, t_command *redir, t_command *cmd)
{
	char	**temp;
	t_redir	*new;

	temp = ft_split(redir->cmd, 0x1F);
	if (!temp[1] || ft_strlen(temp[1]) == 0)
		return (print_error_messages("invalid redirection token",
				NULL, SYNTAX, 0), ft_fsplit(temp), 1);
	if (data()->open_fds < MAX_FD)
	{
		new = new_redir(new_base_redir(redir));
		new->fds.outfile = open(temp[1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (new->fds.outfile < 0)
			return (clear_open_fd_memory(temp, new, cmd));
		ft_fsplit(temp);
		data()->open_fds++;
		add_redir_back(&(cmd->redirs), new);
		remove_node_cmdlst(start, redir);
		return (0);
	}
	ft_fsplit(temp);
	print_error_messages("too many fds are open", NULL, WARNING, 0);
	return (1);
}
