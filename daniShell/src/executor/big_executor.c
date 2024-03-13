/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danalmei <danalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 12:04:31 by danalmei          #+#    #+#             */
/*   Updated: 2024/03/11 09:39:57 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

void	execute_token_is_command(t_command *tmp)
{
	if (!tmp)
		return ;
	if (tmp->is_builtin && !tmp->next && !tmp->prev)
		handle_builtin_execution(tmp);
	else
		execute_cmd(tmp);
}

void	execute_cmd(t_command *tmp)
{
	if (!tmp)
		return ;
	tmp->pid = fork();
	if (tmp->pid < 0)
	{
		perror("fork");
		return ;
	}
	else if (tmp->pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		redirect_stds(tmp);
		close_fds(tmp);
		close_pipeline(tmp);
		child_execution(tmp);
	}
	else
	{
		close_curr_fds(tmp);
		data()->is_executing = 0;
	}
}

void	handle_builtin_execution(t_command *node)
{
	if (!node)
		return ;
	if (node->redirections.outfile > 0
		|| node->redirections.infile > 0)
	{
		node->pid = fork();
		if (node->pid == 0)
		{
			redirect_stds(node);
			data()->last_status_code = builtins(node);
			close_fds(node);
			data_destroy();
			if (data()->hidden_code)
				exit(data()->hidden_code);
			exit(data()->last_status_code);
		}
		else
			return (close_fds(node));
	}
	else
	{
		data()->last_status_code = builtins(node);
		return ;
	}
}

void	execute_token_is_nil(t_command *tmp)
{
	char	*cmd_path;

	if (!tmp)
		return ;
	if (tmp->cmd[0] == '\0')
		cmd_path = NULL;
	else
	{
		cmd_path = get_cmd_path(tmp->cmd, (*env())->get_value(*env(), "PATH"));
		if (cmd_path)
			tmp->cmd_path = cmd_path;
		else
			can_i_make_exec(tmp);
	}
	execute_cmd(tmp);
}

void	can_i_make_exec(t_command *node)
{
	char	**temp;

	temp = ft_split(node->cmd, 0x1F);
	if (access(temp[0], F_OK | X_OK) != 0
		|| (temp[0][0] != '/' && temp[0][0] != '.'))
		return (ft_fsplit(temp));
	node->cmd_path = ft_strdup(temp[0]);
	ft_fsplit(temp);
}
