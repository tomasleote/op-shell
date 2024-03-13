/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danalmei <danalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 14:35:05 by danalmei          #+#    #+#             */
/*   Updated: 2024/03/10 17:40:11 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

static void	set_exit_codes(t_command *start)
{
	t_command	*tmp1;

	tmp1 = start;
	if (data()->hidden_code
		&& data()->last_status_code != data()->hidden_code
		&& !start->pipe)
		data()->last_status_code = data()->hidden_code;
	else
	{
		while (tmp1->next)
			tmp1 = tmp1->next;
		if (tmp1->redirections.outfile == -69)
			data()->last_status_code = data()->hidden_code;
	}
}

void	wait_processes(t_command *start)
{
	int			status;
	t_command	*tmp;

	tmp = start;
	status = -1;
	while (tmp)
	{
		if (tmp->pid != -1)
		{
			waitpid(tmp->pid, &status, 0);
			if (WIFEXITED(status) && WTERMSIG(status) != SIGQUIT)
				data()->last_status_code = WEXITSTATUS(status);
			else if (WTERMSIG(status) == SIGQUIT)
			{
				write(1, "Quit (core dumped)\n", 20);
				data()->last_status_code = 131;
			}
		}
		tmp = tmp->next;
	}
	set_exit_codes(start);
}

int	is_same_file(int fd1, int fd2)
{
	struct stat	stat1;
	struct stat	stat2;

	if (fstat(fd1, &stat1) < 0)
	{
		perror("fstat");
		return (-1);
	}
	if (fstat(fd2, &stat2) < 0)
	{
		perror("fstat");
		return (-1);
	}
	return ((stat1.st_ino == stat2.st_ino) && (stat1.st_dev == stat2.st_dev));
}

int	is_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) != 0)
		return (0);
	return (S_ISDIR(statbuf.st_mode));
}

void	replace_ws_for_space(t_command *node)
{
	int	i;

	i = -1;
	while (node->cmd[++i])
		if (is_whitespace(node->cmd[i]) && node->cmd[i] != 32)
			node->cmd[i] = 32;
}
