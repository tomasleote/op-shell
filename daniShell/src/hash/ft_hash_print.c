/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 19:59:39 by fparreir          #+#    #+#             */
/*   Updated: 2024/01/14 16:15:20 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_hash.h>

static void	ft_hash_print_node(t_hash_node *node, void (*print)(void *ptr),
		t_type type)
{
	if (!node)
		return ;
	if (print)
		print(node->value);
	else
	{
		if (type == STRING)
			printf("%s%s%s\n", BLUE, (char *)node->value, WHITE);
		else if (type == INT)
			printf("%s%d%s\n", BLUE, *(int *)node->value, WHITE);
		else if (type == POINTER)
			printf("%s%p%s\n", BLUE, node->value, WHITE);
		else if (type == DOUBLE || type == FLOAT)
			printf("%s%f%s\n", BLUE, *(double *)node->value, WHITE);
		else if (type == UNSIGNED)
			printf("%s%u%s\n", BLUE, *(unsigned *)node->value, WHITE);
		else if (type == LONG)
			printf("%s%ld%s\n", BLUE, *(long *)node->value, WHITE);
		else if (type == ULONG)
			printf("%s%lu%s\n", BLUE, *(unsigned long *)node->value, WHITE);
		else
			printf("%s(Unknown type printing pointer)%s%p%s\n", RED, BLUE,
				node->value, WHITE);
	}
}

void	ft_hash_print(t_hash *table)
{
	size_t		index;
	t_hash_node	**array;
	t_hash_node	*temp;

	index = 0;
	if (!table)
		return ;
	printf("{\n");
	printf("\tsize = %zu\n", table->size);
	printf("\tcount = %zu\n", table->count);
	array = table->table;
	while (index < table->size)
	{
		temp = array[index];
		while (temp)
		{
			printf("\t%zu -> %s'%s'%s = ", index, YELLOW, temp->key, WHITE);
			ft_hash_print_node(temp, table->print_node, temp->_type);
			temp = temp->next;
		}
		index++;
	}
	printf("}\n");
}
