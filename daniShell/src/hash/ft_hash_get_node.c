/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_get_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 22:14:56 by fparreir          #+#    #+#             */
/*   Updated: 2024/01/14 14:32:41 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash.h>
#include <ft_hash_utils.h>

/**
 * @brief This function returns the node of the hash table that matches the key.
 * @param table pointer to the hash table
 * @param key key to search for
 * @return returns NULL if the key is not found, otherwise returns the node
 */
t_hash_node	*ft_hash_get_node(t_hash *table, const char *key)
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
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}
