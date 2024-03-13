/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:28 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 09:35:10 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <minishell.h>

int	builtins(t_command *node)
{
	int		status;
	char	**args;

	args = ft_split(node->cmd, 0x1F);
	status = execute_builtins(args[0], args, node);
	add_to_env_vars("PWD");
	add_to_env_vars("OLDPWD");
	ft_fsplit(args);
	return (status);
}

int	execute_builtins(char *builtin, char **args, t_command *node)
{
	static t_builtins	data = (t_builtins){
		.cd = &ft_cd, .pwd = &ft_pwd,
		.env = &ft_env, .echo = &ft_echo,
		.exit = &ft_exit, .unset = &ft_unset,
		.export = &ft_export,
	};

	if (ft_strncmp(builtin, "cd", 2) == 0)
		return (data.cd(args));
	else if (ft_strncmp(builtin, "pwd", 3) == 0)
		return (data.pwd(args));
	else if (ft_strncmp(builtin, "env", 3) == 0)
		return (data.env(args));
	else if (ft_strncmp(builtin, "echo", 4) == 0)
		return (data.echo(++args));
	else if (ft_strncmp(builtin, "exit", 4) == 0)
		return (data.exit(args, node));
	else if (ft_strncmp(builtin, "unset", 5) == 0)
		return (data.unset(++args));
	else if (ft_strncmp(builtin, "export", 6) == 0)
		return (data.export(args));
	return (0);
}

void	execute_is_builtin(t_command *cmd)
{
	data()->last_status_code = builtins(cmd);
	if (cmd->next && cmd->redirections.outfile == -1)
		close(STDOUT_FILENO);
	if (cmd->prev && cmd->redirections.infile == -1)
		close(STDIN_FILENO);
	close_fds(cmd);
	close_pipeline(cmd);
	data_destroy();
	exit(data()->last_status_code);
}
