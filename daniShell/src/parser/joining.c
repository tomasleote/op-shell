/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joining.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 21:03:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:08:25 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <utils.h>

int	join_args_and_redirections(t_list **start)
{
	char	*content;
	t_list	*temp;

	if (!start || !*start)
		return (0);
	temp = *start;
	while (temp)
	{
		content = (char *)temp->content;
		if (is_token_redir(temp) > 0)
		{
			if (join_redirections(start, temp, content))
				return (2);
		}
		else
		{
			content = (char *)temp->content;
			if (join_args(temp, content))
				continue ;
		}
		temp = temp->next;
	}
	return (0);
}

int	join_args(t_list *node, char *content)
{
	t_list	*temp2;

	if (!node)
		return (1);
	if (is_token_redir(node->next) == 0)
	{
		content = str_joiner(content, "\x1F", 1);
		node->content = str_joiner(content, (char *)node->next->content, 1);
		temp2 = node->next;
		node->next = node->next->next;
		ft_lstdelone(temp2, free);
		return (1);
	}
	return (0);
}

int	join_redirections(t_list **start, t_list *node, char *content)
{
	t_list	*temp;

	if (!start || !node)
		return (1);
	if ((is_token_redir(node) > 0 && !node->next)
		|| (is_token_redir(node->next) == 1 && is_token_redir(node) < 1))
	{
		print_error_messages("unexpected/invalid token near ",
			node->content, SYNTAX, 0);
		ft_lstclear(start, free);
		return (2);
	}
	else if (is_token_redir(node->next) == 0 && is_token_redir(node) != 1)
	{
		content = str_joiner(content, "\x1F", 1);
		node->content = str_joiner(content, (char *)node->next->content, 1);
		temp = node->next;
		node->next = node->next->next;
		ft_lstdelone(temp, free);
	}
	return (0);
}
