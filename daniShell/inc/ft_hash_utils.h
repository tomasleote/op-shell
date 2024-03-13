/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 18:40:48 by fparreir          #+#    #+#             */
/*   Updated: 2024/01/14 18:55:10 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HASH_UTILS_H
# define FT_HASH_UTILS_H

# include <ft_hash.h>

size_t		hash_djb2(const unsigned char *str);
size_t		get_index(t_hash *table, const unsigned char *key, size_t size);
t_hash_node	*add_node(t_hash *table, const char *key, void *value,
				t_type type);

#endif //FT_HASH_UTILS_H
