/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 22:03:28 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 10:55:19 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>
#include <minishell.h>
#include <parser.h>

int	set_in(t_command *redir, t_command *cmd, t_command **start)
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
		new->fds.infile = open(temp[1], O_RDONLY);
		if (new->fds.infile < 0)
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

int	set_heredoc(t_command **start, t_command *redir, t_command *cmd)
{
	int	pipes[2];

	if (check_max_fds())
		return (1);
	if (pipe(pipes) < 0)
	{
		perror("shitshell: pipe error heredoc");
		return (1);
	}
	data()->open_fds += 2;
	pipeline()->pid = fork();
	if (pipeline()->pid == -1)
	{
		perror("shitshell: fork error heredoc");
		close(pipes[0]);
		close(pipes[1]);
		return (1);
	}
	else if (pipeline()->pid == 0)
		run_here_child(pipes, redir);
	else
		run_here_parent(pipes, start, redir, cmd);
	return (0);
}
