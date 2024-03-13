/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_delete.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 20:32:04 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/21 17:12:34 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash.h>

void	ft_hash_delete(t_hash *table)
{
	size_t		i;
	t_hash_node	*temp;
	t_hash_node	*temp_next;

	if (!table)
		return ;
	i = 0;
	while (i < table->size)
	{
		temp = table->table[i];
		while (temp)
		{
			temp_next = temp->next;
			free(temp->key);
			if (temp->value)
				table->cleanup(temp->value);
			free(temp);
			temp = temp_next;
		}
		i++;
	}
	free(table->table);
	free(table);
}
