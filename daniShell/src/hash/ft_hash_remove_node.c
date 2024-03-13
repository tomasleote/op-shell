/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_remove_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 13:47:16 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/28 17:43:09 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash.h>
#include <ft_hash_utils.h>

static int	remove_node(t_hash *table, t_hash_node *node)
{
	if (node->value)
		table->cleanup(node->value);
	free(node->key);
	free(node);
	table->count--;
	return (1);
}

int	ft_hash_remove_node(t_hash *table, const char *key)
{
	size_t		index;
	t_hash_node	*node;
	t_hash_node	*prev;

	if (!table || !key || !(*key))
		return (0);
	prev = NULL;
	index = get_index(table, (const unsigned char *)key, table->size);
	node = table->table[index];
	while (node)
	{
		if (ft_strncmp(key, node->key, ft_strlen(key)) == 0)
		{
			if (prev)
				prev->next = node->next;
			else
				table->table[index] = node->next;
			return (remove_node(table, node));
		}
		prev = node;
		node = node->next;
	}
	return (0);
}
