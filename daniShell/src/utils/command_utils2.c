/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/03 20:04:16 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:09:24 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

void	print_cmd(t_command *node)
{
	if (!node)
		return ;
	printf("  %scmd%s: %s%s%s,\n", YELLOW, WHITE, GREEN, node->cmd, WHITE);
	printf("  %scmd_path%s: %s%s%s,\n", YELLOW, WHITE, GREEN, node->cmd_path,
		WHITE);
	printf("  %scurr%s: %s%p%s,\n", YELLOW, WHITE, BLUE, node, WHITE);
	printf("  %sprev%s: %s%p%s,\n", YELLOW, WHITE, PURPLE, node->prev, WHITE);
	printf("  %snext%s: %s%p%s,\n", YELLOW, WHITE, PURPLE, node->next, WHITE);
	printf("  %stoken%s: %s\n", YELLOW, WHITE, show_token(node->token));
	printf("  %sbuiltin%s: %s\n", YELLOW, WHITE, show_bool(node->is_builtin));
	printf("  %sexpansions%s: %s\n", YELLOW, WHITE, show_bool(node->expand));
	printf("  %spipe%s: %s\n", YELLOW, WHITE, show_bool(node->pipe));
	printf("  %spipes%s: { write: %d, read: %d }\n", YELLOW, WHITE,
		node->pipes[1], node->pipes[0]);
	printf("  %sredirection%s: { in: %d, out: %d }\n", YELLOW, WHITE,
		node->redirections.infile, node->redirections.outfile);
	printf("  %sredirection_list%s: ", YELLOW, WHITE);
	if (node->redirs)
	{
		print_redir_lst(node->redirs);
		printf("\n");
	}
	else
		printf("%s\n", show_bool(false));
}

void	print_cmd_lst(t_command *node)
{
	t_command	*temp;

	if (!node)
	{
		printf("%sEMPTY%s\n", RED, WHITE);
		return ;
	}
	temp = node;
	while (temp->prev)
		temp = temp->prev;
	while (temp)
	{
		printf("%s{%s\n", PURPLE, WHITE);
		print_cmd(temp);
		if (temp->next)
			printf("%s}%s,\n", PURPLE, WHITE);
		else
			printf("%s}%s\n", PURPLE, WHITE);
		temp = temp->next;
	}
}

static void	close_node_fds(t_command *node)
{
	if (node->pipes[0] > -1)
		close(node->pipes[0]);
	if (node->pipes[1] > -1)
		close(node->pipes[1]);
	if (node->redirections.outfile > -1)
		close(node->redirections.outfile);
	if (node->redirections.infile > -1)
		close(node->redirections.infile);
}

void	remove_node_cmdlst(t_command **start, t_command *node)
{
	if (!start || !*start || !node)
		return ;
	if (*start == node)
	{
		*start = node->next;
		if (node->next != NULL)
			node->next->prev = NULL;
	}
	else
	{
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;
	}
	if (node->cmd_path)
		free(node->cmd_path);
	if (node->redirs)
		clear_redir_lst(node->redirs);
	close_node_fds(node);
	free(node->cmd);
	free(node);
}

t_command	new_empty_cmd(void)
{
	return ((t_command){
		.cmd_path = NULL,
		.cmd = NULL,
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
