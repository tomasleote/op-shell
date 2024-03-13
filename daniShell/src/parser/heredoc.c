/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:19:40 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/08 14:09:32 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <parser.h>
#include <utils.h>

void	run_here_child(int pipe[2], t_command *reddirection)
{
	int	exit_code;

	signal(SIGINT, sig_handle_doc);
	pipeline()->temp_fds.infile = pipe[1];
	pipeline()->temp_fds.outfile = pipe[0];
	pipeline()->temp = reddirection;
	(pipeline())->char_array = ft_split(pipeline()->temp->cmd, 0x1F);
	exit_code = fill_heredoc(pipeline()->temp_fds.infile,
			pipeline()->char_array[1]);
	pipeline_destroy();
	data_destroy();
	exit(exit_code);
}

void	run_here_parent(int pipes[2], t_command **start,
		t_command *redir, t_command *cmd)
{
	t_redir	*new;
	int		status;

	close(pipes[1]);
	while (waitpid(pipeline()->pid, &status, 0) > 0)
		;
	if (WIFEXITED(status))
		data()->last_status_code = WEXITSTATUS(status);
	new = new_redir(new_base_redir(redir));
	new->fds.infile = pipes[0];
	add_redir_back(&(cmd->redirs), new);
	remove_node_cmdlst(start, redir);
	data()->open_fds -= 2;
}

//TODO: swap to void?
int	fill_heredoc(int fd, char *eof)
{
	char	*line;
	int		ret;

	ret = 0;
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			print_error_messages("heredoc was closed manually",
				NULL, WARNING, 0);
			break ;
		}
		else if (ft_strncmp(line, eof, ft_strlen(eof)) == 0
			&& ft_strlen(line) == ft_strlen(eof))
		{
			free(line);
			break ;
		}
		line = expand_env_var_heredoc(line);
		line = str_joiner(line, "\n", 1);
		write(fd, line, ft_strlen(line));
		free(line);
	}
	return (ret);
}
