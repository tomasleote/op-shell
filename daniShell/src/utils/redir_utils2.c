/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 17:39:44 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/13 17:40:43 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

void	print_redir_lst(t_redir *redir)
{
	t_redir	*temp;

	if (!redir)
		return ;
	temp = redir;
	while (temp->prev)
		temp = temp->prev;
	while (temp)
	{
		printf("{\n");
		printf("type = %s,\n", show_token(temp->type));
		printf("fds = { in: %d, out: %d }, \n", temp->fds.infile,
			temp->fds.outfile);
		printf("prev = %p, \n", temp->prev);
		printf("next = %p \n", temp->next);
		printf("}\n");
		temp = temp->next;
	}
}
