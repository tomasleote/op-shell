/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:14:44 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 11:02:45 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>
#include <parser.h>

t_pipeline	*pipeline(void)
{
	static t_pipeline	ptr;

	return (&ptr);
}

t_command	**pipeline_init(t_command **start, int size)
{
	(pipeline())->array = create_pipeline(start, size);
	pipeline()->size = size;
	pipeline()->temp = NULL;
	pipeline()->char_array = NULL;
	(pipeline())->temp_fds = (t_fds){-1, -1};
	return (pipeline()->array);
}

t_command	**create_pipeline(t_command **original, int size)
{
	t_command	**res;
	t_command	*temp1;
	t_command	*temp2;
	int			i;

	i = 0;
	res = ft_calloc(size + 1, sizeof(t_command *));
	temp1 = *original;
	while (temp1)
	{
		temp2 = temp1;
		while (temp2 && temp2->token != PIPE)
		{
			add_command_back(&(res[i]), clone_command(temp2));
			temp2 = temp2->next;
		}
		i++;
		if (temp2)
			temp1 = temp2->next;
		else
			temp1 = temp2;
	}
	res[i] = 0;
	return (res);
}

static void	clear_pipeline(void)
{
	int	i;

	i = 0;
	if (pipeline()->array != NULL)
	{
		while (i < pipeline()->size)
		{
			if (pipeline()->array[i])
			{
				clear_cmd_lst(pipeline()->array[i]);
				pipeline()->array[i] = NULL;
			}
			i++;
		}
		free(pipeline()->array);
		pipeline()->array = NULL;
	}
}

void	pipeline_destroy(void)
{
	clear_pipeline();
	if (pipeline()->char_array)
	{
		ft_fsplit(pipeline()->char_array);
		pipeline()->char_array = NULL;
	}
	if (pipeline()->temp)
		pipeline()->temp = NULL;
	if (pipeline()->temp_fds.outfile > 0)
	{
		close(pipeline()->temp_fds.outfile);
		pipeline()->temp_fds.outfile = -1;
	}
	if (pipeline()->temp_fds.infile > 0)
	{
		close(pipeline()->temp_fds.infile);
		pipeline()->temp_fds.infile = -1;
	}
}
