/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_get_item.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 19:51:38 by fparreir          #+#    #+#             */
/*   Updated: 2024/01/14 14:36:41 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash.h>
#include <ft_hash_utils.h>

void	*ft_hash_get_item(t_hash *table, const char *key)
{
	size_t		index;
	t_hash_node	**array;
	t_hash_node	*temp;

	if (!table)
		return (NULL);
	index = get_index(table, (const unsigned char *)key, table->size);
	array = table->table;
	temp = array[index];
	while (temp)
	{
		if (ft_strncmp(key, temp->key, ft_strlen(key)) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}
