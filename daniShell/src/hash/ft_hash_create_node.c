/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_create_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 18:48:58 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/21 17:11:38 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash.h>

t_hash_node	*ft_hash_create_node(const char *key, void *value)
{
	t_hash_node	*new;

	new = malloc(sizeof(t_hash_node));
	if (!new)
		return (NULL);
	if (key)
		new->key = ft_strdup(key);
	else
		new->key = NULL;
	new->value = value;
	new->_type = POINTER;
	new->source = NULL;
	new->next = NULL;
	return (new);
}
