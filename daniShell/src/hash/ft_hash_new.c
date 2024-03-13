/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_new.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 18:38:11 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/23 13:56:55 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash.h>
#include <ft_hash_utils.h>

/**
 * @brief This function sets the default builtin functions for the hash table.
 * @param table pointer to the hash table
 * @param hash hash function pointer
 * @param cleanup node cleanup function pointer
 * @param print node print function pointer
 */
static void	set_builtins(t_hash *table,
		size_t (*hash)(const unsigned char *ptr), void (*cleanup)(void *ptr),
		void (*print)(void *ptr))
{
	table->get_node = &ft_hash_get_node;
	table->get_value = &ft_hash_get_item;
	table->print = &ft_hash_print;
	table->add = add_node;
	table->delete = &ft_hash_remove_node;
	table->clean = &ft_hash_delete;
	if (print)
		table->print_node = print;
	else
		table->print_node = NULL;
	if (!hash)
		table->hash = &hash_djb2;
	else
		table->hash = hash;
	if (!cleanup)
		table->cleanup = &free;
	else
		table->cleanup = cleanup;
}

/**
 * @brief This function allocates and creates a new hash table.
 * @param size size of the hash table
 * @param hash function pointer containing an hash function
 * @param cleanup function pointer containing an node cleanup function
 * @param print function pointer containing an node print function
 * @return returns NULL if memory allocation fails, otherwise
 * returns the new hash table
 *
 * NOTE: The hash table contains a set of default values baked into the library.
 * This means that the hash, cleanup and print function pointers can be NULL.
 * If they are NULL, the default values will be used instead.
 * The default hash function is djb2.
 */
t_hash	*ft_hash_create(size_t size, size_t (*hash)(const unsigned char *ptr),
		void (*cleanup)(void *ptr), void (*print)(void *ptr))
{
	t_hash	*new;

	new = malloc(sizeof(t_hash));
	if (!new)
		return (NULL);
	new->table = ft_calloc(sizeof(new->table), size);
	if (!new->table)
		return (free(new), NULL);
	new->size = size;
	new->count = 0;
	set_builtins(new, hash, cleanup, print);
	return (new);
}
