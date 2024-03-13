/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 15:14:38 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 10:58:52 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

int	ft_access(char *arg)
{
	if (!arg)
		return (1);
	if (access(arg, F_OK | R_OK) == -1)
	{
		perror("shitshell: cd");
		return (1);
	}
	return (0);
}

int	count_args(char **args)
{
	int	res;

	res = 0;
	while (args[res])
		res++;
	return (res);
}

int	is_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] != '-' && str[i] != '+') && !ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

void	close_curr_fds(t_command *tmp)
{
	int	temp;

	temp = -1;
	if (tmp->next)
		close(tmp->pipes[1]);
	if (tmp->prev)
		close(tmp->prev->pipes[0]);
	if (tmp->redirections.infile != -1)
	{
		if (is_same_file(tmp->redirections.infile, data()->null))
			temp = -69;
		close(tmp->redirections.infile);
		tmp->redirections.infile = temp;
	}
	if (tmp->redirections.outfile != -1)
	{
		if (is_same_file(tmp->redirections.outfile, data()->null))
			temp = -69;
		close(tmp->redirections.outfile);
		tmp->redirections.outfile = temp;
	}
}

void	copy_default_stds(t_command *cmd, int fds[2])
{
	if (cmd->prev && cmd->redirections.infile == -1)
	{
		fds[0] = dup(STDIN_FILENO);
		dup2(cmd->prev->pipes[0], STDIN_FILENO);
	}
	if (cmd->next && cmd->redirections.outfile == -1)
	{
		fds[1] = dup(STDOUT_FILENO);
		dup2(cmd->pipes[1], STDOUT_FILENO);
	}
}
