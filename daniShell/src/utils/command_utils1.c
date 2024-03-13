/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 20:04:16 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:09:16 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <environment.h>
#include <utils.h>

t_command	new_base_cmd(t_list *node)
{
	char	*cmd;
	char	*cmd_path;

	cmd = (char *)node->content;
	cmd_path = get_cmd_path(cmd, ft_hash_get_item(*env(), "PATH"));
	return ((t_command){
		.cmd_path = cmd_path,
		.cmd = ft_strdup(cmd),
		.is_builtin = false,
		.pipe = false,
		.expand = false,
		.redirs = NULL,
		.pid = 0,
		.pipes = {-1, -1},
		.redirections = {-1, -1},
		.token = NIL,
		.prev = NULL,
		.next = NULL,
	});
}

t_command	*new_command(t_command command)
{
	t_command	*res;

	res = malloc(sizeof(t_command));
	if (!res)
		return (res);
	*res = command;
	return (res);
}

t_command	*add_command_back(t_command **start, t_command *command)
{
	t_command	*temp;

	if (!start || !command)
		return (NULL);
	temp = *start;
	if (!*start)
	{
		*start = command;
		return (command);
	}
	while (temp->next)
		temp = temp->next;
	temp->next = command;
	command->prev = temp;
	command->next = NULL;
	return (command);
}

void	clear_cmd_lst(t_command *node)
{
	t_command	*temp;
	t_command	*temp2;

	temp = node;
	if (temp->prev != NULL)
		while (temp->prev)
			temp = temp->prev;
	while (temp)
	{
		temp2 = temp->next;
		if (temp->cmd_path)
			free(temp->cmd_path);
		if (temp->redirs)
			clear_redir_lst(temp->redirs);
		if (temp->redirections.infile > 0)
			close(temp->redirections.infile);
		if (temp->redirections.outfile > 0)
			close(temp->redirections.outfile);
		free(temp->cmd);
		free(temp);
		temp = temp2;
	}
}

void	add_node_between_cmds(t_command **start, t_command *to_add,
			t_command *after)
{
	t_command	*temp;

	if (!start || !*start || !to_add || !after)
		return ;
	temp = *start;
	while (temp && temp != after)
		temp = temp->next;
	if (!temp)
		return ;
	to_add->next = temp->next;
	if (temp->next)
		temp->next->prev = to_add;
	temp->next = to_add;
	to_add->prev = temp;
}
