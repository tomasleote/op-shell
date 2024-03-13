/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tiny_executors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danalmei <danalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 22:28:22 by danalmei          #+#    #+#             */
/*   Updated: 2024/03/10 17:44:47 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <environment.h>
#include <minishell.h>

void	child_execution(t_command *cmd)
{
	execute_no_args(cmd);
	if (cmd->is_builtin == false)
	{
		if (cmd->cmd_path)
			execute_has_cmd_path(cmd);
		else
			execute_has_no_cmd_path(cmd);
	}
	else
		execute_is_builtin(cmd);
}

void	execute_no_args(t_command *cmd)
{
	char	**args;
	int		fds[2];
	char	*path;

	fds[0] = -1;
	fds[1] = -1;
	if (cmd->is_builtin)
		return ;
	path = (*env())->get_value(*env(), "PATH");
	args = ft_split(cmd->cmd, 0x1F);
	if ((!cmd->cmd_path && (!args || args[0] == NULL
				|| (path && args[0][0] != '/' && args[0][0] != '.'
			&& access(cmd->cmd, F_OK | X_OK) == 0))
			&& !cmd->expand))
	{
		put_err_msg(cmd->cmd, get_err_msg(0, NULL));
		copy_default_stds(cmd, fds);
		restore_stds(fds);
		ft_fsplit(args);
		close_pipeline(cmd);
		data_destroy();
		exit(127);
	}
	return (ft_fsplit(args));
}

void	execute_has_cmd_path(t_command *cmd)
{
	char	**args;

	args = ft_split(cmd->cmd, 0x1F);
	if (!args)
		return ;
	if (execve(cmd->cmd_path, args, data()->envp) == -1)
		handle_execve_error(cmd, args, errno);
	ft_fsplit(args);
}

static void	fuck_norm1(t_command *cmd)
{
	char	**args;

	replace_ws_for_space(cmd);
	args = ft_split(cmd->cmd, 32);
	if (!args)
		return ;
	cmd->cmd_path = get_cmd_path(args[0], (*env())->get_value(*env(), "PATH"));
	if (cmd->cmd_path)
	{
		if (execve(cmd->cmd_path, args, data()->envp) == -1)
			handle_execve_error(cmd, args, errno);
	}
	else
		handle_execve_error(cmd, args, errno);
}

void	execute_has_no_cmd_path(t_command *cmd)
{
	char	**args;

	if (access(cmd->cmd, F_OK | X_OK) == 0)
	{
		args = ft_split(cmd->cmd, 0x1F);
		if (!args)
			return ;
		if (execve(cmd->cmd, args, data()->envp) == -1)
			handle_execve_error(cmd, args, errno);
	}
	else
		fuck_norm1(cmd);
}
