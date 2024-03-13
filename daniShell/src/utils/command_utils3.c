/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 13:05:50 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/16 16:14:48 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

int	count_pipes(t_command *node)
{
	int	res;

	res = 0;
	if (!node)
		return (res);
	while (node->prev)
		node = node->prev;
	while (node)
	{
		if (node->token == PIPE)
			res++;
		node = node->next;
	}
	return (res + 1);
}

int	cmd_lst_size(t_command *node)
{
	int	res;

	res = 0;
	if (!node)
		return (res);
	while (node->prev)
		node = node->prev;
	while (node)
	{
		res++;
		node = node->next;
	}
	return (res);
}

t_command	*clone_command(t_command *node)
{
	t_command	*res;

	res = malloc(sizeof(t_command));
	if (!res)
		return (res);
	*res = new_empty_cmd();
	res->token = node->token;
	res->expand = node->expand;
	res->is_builtin = node->is_builtin;
	if (node->cmd_path)
		res->cmd_path = ft_strdup(node->cmd_path);
	if (node->cmd)
		res->cmd = ft_strdup(node->cmd);
	return (res);
}

void	move_node_to_lst(t_command **dst, t_command *to_move)
{
	t_command	*temp;

	if (!*dst)
	{
		*dst = to_move;
		(*dst)->next = NULL;
		(*dst)->prev = NULL;
	}
	else
	{
		temp = *dst;
		while (temp->next)
			temp = temp->next;
		temp->next = to_move;
		to_move->prev = temp;
		to_move->next = NULL;
	}
}

void	delete_cmd_node(t_command *node)
{
	t_command	*temp1;
	t_command	*temp2;

	if (!node)
		return ;
	temp1 = node->prev;
	temp2 = node->next;
	if (temp1)
		temp1->next = temp2;
	if (temp2)
		temp2->prev = temp1;
	if (node->cmd)
		free(node->cmd);
	if (node->cmd_path)
		free(node->cmd_path);
	if (node->redirs)
		clear_redir_lst(node->redirs);
	if (node->redirections.outfile > 0)
		close(node->redirections.outfile);
	if (node->redirections.infile > 0)
		close(node->redirections.infile);
	free(node);
}
