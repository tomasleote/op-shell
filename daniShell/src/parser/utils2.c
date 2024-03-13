/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:09:09 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <utils.h>
#include <minishell.h>

void	handle_expansion_quotes(t_command *cmd)
{
	int		i;
	bool	dquote;
	bool	squote;

	if (!cmd)
		return ;
	i = -1;
	dquote = false;
	squote = false;
	while (cmd->cmd[++i])
	{
		if (is_dquote(cmd->cmd[i]) && !squote)
			dquote = true;
		else if (is_squote(cmd->cmd[i]) && !dquote)
			squote = true;
		else if (is_dollar(cmd->cmd[i]))
		{
			if ((cmd->cmd[i + 1] && ft_isalnum(cmd->cmd[i + 1]))
				|| (cmd->cmd[i + 1] == '_' || cmd->cmd[i + 1] == '?'))
				if ((dquote && !squote) || (!dquote && !squote))
					cmd->expand = true;
		}
	}
}

int	is_token_redir(t_list *node)
{
	char	*content;

	if (!node)
		return (-1);
	content = (char *)node->content;
	if (ft_strncmp(content, "|", 1) == 0)
		return (1);
	else if (ft_strncmp(content, ">", 1) == 0)
		return (2);
	else if (ft_strncmp(content, "<", 1) == 0)
		return (3);
	else if (ft_strncmp(content, "<<", 2) == 0)
		return (4);
	else if (ft_strncmp(content, ">>", 2) == 0)
		return (5);
	else
		return (0);
}

int	detect_redirection_tokens(t_command *node)
{
	char	**temp;

	temp = ft_split(node->cmd, 0x1F);
	if (!temp)
		return (1);
	if (ft_strncmp(temp[0], "<<", 2) == 0)
		node->token = HEREDOC;
	else if (ft_strncmp(temp[0], ">>", 2) == 0)
		node->token = APPEND;
	else if (ft_strncmp(temp[0], "<", 1) == 0)
		node->token = IN;
	else if (ft_strncmp(temp[0], ">", 1) == 0)
		node->token = OUT;
	if (!temp[1] && (node->token == HEREDOC || node->token == APPEND
			|| node->token == IN || node->token == OUT))
	{
		print_error_messages("unexpected/invalid token near ",
			temp[0], SYNTAX, 0);
		clear_cmd_lst(node);
		ft_fsplit(temp);
		return (2);
	}
	ft_fsplit(temp);
	return (0);
}

void	dup_ins_outs(t_command *node)
{
	if (!node)
		return ;
	if (data()->open_fds <= MAX_FD)
	{
		if (node->redirections.infile == data()->null)
		{
			node->redirections.outfile = dup(data()->null);
			node->redirections.infile = dup(data()->null);
			return ;
		}
		if (node->redirections.infile > 0)
			node->redirections.infile = dup(node->redirections.infile);
		if (node->redirections.outfile > 0)
			node->redirections.outfile = dup(node->redirections.outfile);
	}
}

void	set_inputs_and_outs(t_command *node)
{
	t_redir	*temp;

	if (!node || !node->redirs)
		return ;
	temp = node->redirs;
	while (temp)
	{
		if (node->redirections.infile != data()->null
			&& (temp->type == HEREDOC || temp->type == IN))
			node->redirections.infile = temp->fds.infile;
		else if (node->redirections.outfile != data()->null
			&& (temp->type == APPEND || temp->type == OUT))
			node->redirections.outfile = temp->fds.outfile;
		temp = temp->next;
	}
}
