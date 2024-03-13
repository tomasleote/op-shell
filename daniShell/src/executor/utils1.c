/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danalmei <danalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 13:44:28 by danalmei          #+#    #+#             */
/*   Updated: 2024/03/11 11:00:19 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

void	open_pipeline(t_command *start)
{
	t_command	*tmp;

	tmp = start;
	if (tmp->pipes[0] != -1 && tmp->pipes[1] != -1)
		return ;
	while (tmp && tmp->next)
	{
		if (pipe(tmp->pipes) == -1)
		{
			perror("pipe");
			return ;
		}
		data()->open_fds += 2;
		tmp = tmp->next;
	}
}

void	close_pipeline(t_command *start)
{
	t_command	*tmp;

	tmp = start;
	while (tmp->prev)
		tmp = tmp->prev;
	while (tmp->next)
	{
		close(tmp->pipes[0]);
		close(tmp->pipes[1]);
		data()->open_fds -= 2;
		tmp = tmp->next;
	}
}

void	close_fds(t_command *start)
{
	t_command	*tmp;

	tmp = start;
	while (tmp->prev)
		tmp = tmp->prev;
	while (tmp)
	{
		if (tmp->redirections.outfile > 0)
			close(tmp->redirections.outfile);
		if (tmp->redirections.infile > 0)
			close(tmp->redirections.infile);
		tmp = tmp->next;
	}
	close(data()->null);
}

void	redirect_stds(t_command	*tmp)
{
	if (tmp->redirections.outfile > 0)
	{
		dup2(tmp->redirections.outfile, STDOUT_FILENO);
		close(tmp->redirections.outfile);
	}
	else if (tmp->next)
	{
		dup2(tmp->pipes[1], STDOUT_FILENO);
		close(tmp->pipes[1]);
	}
	if (tmp->redirections.infile > 0)
	{
		dup2(tmp->redirections.infile, STDIN_FILENO);
		close(tmp->redirections.infile);
	}
	else if (tmp->prev)
	{
		dup2(tmp->prev->pipes[0], STDIN_FILENO);
		close(tmp->prev->pipes[0]);
	}
}

void	restore_stds(int fds[2])
{
	if (fds[0] != -1)
	{
		if (dup2(fds[0], STDIN_FILENO) == -1)
			perror("Failed to restore STDIN");
		close(fds[0]);
	}
	if (fds[1] != -1)
	{
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			perror("Failed to restore STDOUT");
		close(fds[1]);
	}
}
