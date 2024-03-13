/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 18:04:35 by fparreir          #+#    #+#             */
/*   Updated: 2024/02/23 13:58:52 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//This is an library based on this guide: https://bit.ly/4aYgf9m
// However there were made a lot of tweaks to it in order to make it something
// more robust, the library itself should allow for type safety, however,
// that will only happen if the initialization is done properly and the
// function pointers given are properly tested. This means that if you
// set the table to work with strings you can only add strings to it, otherwise
// behaviour is undefined.
// Finally while this library has some garbage collection implemented
// you are always responsible for giving proper cleanup functions for complex
// data structures, such as linked lists, trees, etc.
#ifndef FT_HASH_H
# define FT_HASH_H

# define BLACK		"\033[0;30m"
# define RED		"\033[0;31m"
# define GREEN		"\033[0;32m"
# define YELLOW		"\033[0;33m"
# define BLUE		"\033[0;34m"
# define PURPLE		"\033[0;35m"
# define CYAN		"\033[0;36m"
# define WHITE		"\033[0m"
# define MAGENTA	"\e[0;35m"

# include <libft.h>

/**
 * @brief This struct represents a node in the hash table.
 * @param key key of the node
 * @param value value of the node
 * @param type type of the value
 * @param source pointer to the hash table
 * @param next pointer to the next node
 * @param print function pointer containing an node print function
 */
typedef struct s_hash_node		t_hash_node;

/**
 * @brief This struct represents a hash table.
 * @param size size of the hash table
 * @param count number of nodes in the hash table
 * @param table pointer to the hash table
 * @param clean function pointer containing an hash table cleanup function
 * @param cleanup function pointer containing an node
 * cleanup function (optional)
 * @param print function pointer containing an hash table print function
 * @param print_node function pointer containing an node
 * print function (optional)
 * @param get_value function pointer containing an node get value function
 * @param hash function pointer containing an hash function (optional)
 * @param add function pointer containing an node add function
 * @param get_node function pointer containing an node get function
 */
typedef struct s_hash			t_hash;

/**
 * @brief This enum represents the type of data we are storing.
 * @param STRING string ('s')
 * @param INT integer ('i')
 * @param DOUBLE double ('d')
 * @param FLOAT float ('f')
 * @param UNSIGNED unsigned integer ('u')
 * @param LONG long ('l')
 * @param ULONG unsigned long ('z')
 * @param POINTER pointer ('p')
 */
typedef enum e_type				t_type;

enum e_type {
	STRING = 's',
	INT = 'i',
	DOUBLE = 'd',
	FLOAT = 'f',
	UNSIGNED = 'u',
	LONG = 'l',
	ULONG = 'z',
	POINTER = 'p',
};

struct s_hash_node {
	char		*key;
	void		*value;
	t_type		_type;
	t_hash		*source;
	t_hash_node	*next;

	void		(*print)(void *node);
};

struct s_hash {
	size_t		size;
	size_t		count;
	t_hash_node	**table;

	void		(*clean)(t_hash *table);
	void		(*cleanup)(void *table);
	void		(*print)(t_hash	*table);
	void		(*print_node)(void	*ptr);
	void		*(*get_value)(t_hash *table, const char *key);
	int			(*delete)(t_hash *table, const char *key);
	size_t		(*hash)(const unsigned char *key);
	t_hash_node	*(*add)(t_hash *table, const char *key, void *value,
			t_type type);
	t_hash_node	*(*get_node)(t_hash *table, const char *key);
};

int			ft_hash_remove_node(t_hash *table, const char *key);
int			ft_hash_add_node(t_hash *table, const char *key, void *value,
				t_type type);
void		*ft_hash_get_item(t_hash *table, const char *key);
void		ft_hash_print(t_hash *table);
void		ft_hash_delete(t_hash *table);
t_hash		*ft_hash_create(size_t size, size_t (*hash)(const unsigned char *),
				void (*cleanup)(void *), void (*print)(void *ptr));
t_hash_node	*ft_hash_get_node(t_hash *table, const char *key);
t_hash_node	*ft_hash_create_node(const char *key, void *value);

#endif //FT_HASH_H
