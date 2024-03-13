/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_hash_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 18:52:29 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/25 17:34:15 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash.h>
#include <ft_hash_utils.h>

/**
 * @brief This function updates the value of a node if it already exists.
 * @param node
 * @param key
 * @param value
 * @return
 */
static int	update_if_exists(t_hash_node *node, const char *key,
		void *value)
{
	while (node)
	{
		if (ft_strncmp(key, node->key, ft_strlen(key)) == 0)
		{
			if (node->value != NULL)
				(node->source)->cleanup(node->value);
			node->value = value;
			node->source->count++;
			return (1);
		}
		node = node->next;
	}
	return (0);
}

/**
 * @brief This function adds a node to the hash table.
 * @param table pointer to the hash table
 * @param key key to add
 * @param value value to add
 * @param type type of the value
 * @return returns 1 if the node was added, otherwise returns 0
 */
int	ft_hash_add_node(t_hash *table, const char *key, void *value, t_type type)
{
	size_t		index;
	t_hash_node	*node;
	t_hash_node	**array;

	if (!table || ((!key || !(*key))))
		return (0);
	array = table->table;
	index = get_index(table, (const unsigned char *)key, table->size);
	if (update_if_exists(array[index], key, value))
		return (1);
	node = ft_hash_create_node(key, value);
	if (!node)
		return (0);
	node->source = table;
	table->count++;
	node->_type = type;
	if (array[index])
	{
		node->next = array[index];
		array[index] = node;
	}
	else
		array[index] = node;
	return (1);
}
