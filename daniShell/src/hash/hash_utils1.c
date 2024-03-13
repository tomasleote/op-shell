/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 18:42:09 by fparreir          #+#    #+#             */
/*   Updated: 2024/01/14 18:51:52 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash_utils.h>

// explanation on this algorithm:
// https://theartincode.stanis.me/008-djb2/
size_t	hash_djb2(const unsigned char *str)
{
	int		c;
	size_t	hash;

	hash = 5381;
	while (str && *str++)
	{
		c = *str;
		hash = ((hash << 5) + c);
	}
	return (hash);
}

/**
 * @brief This function returns the index of the key in the hash table.
 * @param table table pointer used to grab the hash function
 * @param key byte array containing the key
 * @param size size of the table
 * @return returns the index of the key in the hash table
 */
size_t	get_index(t_hash *table, const unsigned char *key, size_t size)
{
	size_t	index;

	index = table->hash(key) % size;
	return (index);
}

/**
 * @brief This function is a built-in function that adds
 * a node to the hash table.
 * @param table pointer to the hash table
 * @param key pointer to the key value
 * @param value pointer to the object we want to store (must be heap allocated)
 * @param type type of data we are storing
 * @return returns the newly created node, or NULL if something went wrong
 */
t_hash_node	*add_node(t_hash *table, const char *key, void *value, t_type type)
{
	t_hash_node	*node;

	if (ft_hash_add_node(table, key, value, type))
	{
		node = ft_hash_get_node(table, key);
		if (!node)
			return (NULL);
		return (node);
	}
	return (NULL);
}
