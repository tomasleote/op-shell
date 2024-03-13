/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 15:01:59 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 11:01:55 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

t_redir	new_base_redir(t_command *node)
{
	return ((t_redir){
		.type = node->token,
		.fds = {-1, -1},
		.next = NULL,
		.prev = NULL,
	});
}

t_redir	*new_redir(t_redir redir)
{
	t_redir	*res;

	res = malloc(sizeof(t_redir));
	if (!res)
		return (res);
	*res = redir;
	return (res);
}

t_redir	*add_redir_back(t_redir **start, t_redir *node)
{
	t_redir	*temp;

	if (!start)
		return (NULL);
	if (!*start)
	{
		*start = node;
		return (node);
	}
	temp = *start;
	while (temp->next)
		temp = temp ->next;
	temp->next = node;
	node->prev = temp;
	node->next = NULL;
	return (node);
}

void	clear_redir_lst(t_redir *node)
{
	t_redir	*temp;
	t_redir	*temp2;

	if (!node)
		return ;
	temp = node;
	if (temp->prev != NULL)
		while (temp->prev)
			temp = temp->prev;
	while (temp)
	{
		temp2 = temp->next;
		if (temp->fds.infile > 0)
			close(temp->fds.infile);
		if (temp->fds.outfile > 0)
			close(temp->fds.outfile);
		free(temp);
		temp = temp2;
	}
}

void	remove_node_redir_lst(t_redir **start, t_redir *node)
{
	if (!start || !*start || !node)
		return ;
	if (*start == node)
	{
		*start = node->next;
		if (*start)
			(*start)->prev = NULL;
	}
	else
	{
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;
	}
	if (node->fds.infile > 0)
		close(node->fds.infile);
	if (node->fds.outfile > 0)
		close(node->fds.outfile);
	free(node);
}
