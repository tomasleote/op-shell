/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:08:50 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <utils.h>
#include <parser.h>
#include <minishell.h>

int	parse_cmd_lst(t_command *start)
{
	t_command	*temp;

	if (!start)
		return (1);
	temp = start;
	while (temp)
	{
		if (temp->cmd && ft_strncmp(temp->cmd, "|", 1) == 0)
			temp->token = PIPE;
		else if (temp->cmd_path)
			temp->token = COMMAND;
		else if (!temp->cmd_path)
		{
			if (detect_redirection_tokens(temp))
			{
				data()->last_status_code = 1;
				return (2);
			}
		}
		temp = temp->next;
	}
	return (0);
}

void	parse_expansions(t_command *start)
{
	t_command	*temp;

	if (!start)
		return ;
	temp = start;
	while (temp != NULL)
	{
		if (temp->token != HEREDOC && temp->token != PIPE)
			handle_expansion_quotes(temp);
		temp = temp->next;
	}
}

void	parse_builtins(t_command *start)
{
	char		**temp;
	t_command	*ptr;

	if (!start)
		return ;
	ptr = start;
	while (ptr)
	{
		temp = ft_split(ptr->cmd, 0x1F);
		if (!temp)
			return ;
		if (is_builtin(temp[0]))
		{
			if (ptr->cmd_path)
			{
				free(ptr->cmd_path);
				ptr->cmd_path = NULL;
			}
			ptr->is_builtin = true;
			ptr->token = COMMAND;
		}
		ft_fsplit(temp);
		ptr = ptr->next;
	}
}
