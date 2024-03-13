/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:24:21 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 16:04:31 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <minishell.h>
#include <utils.h>

void	parser(void)
{
	data()->commands = parse_input(&(data()->lexer));
	expand(data()->commands);
	data()->on_heredoc = 1;
	setup_redirs(&(data())->commands);
	data()->on_heredoc = 0;
	stop_assholes(data()->commands);
}

t_command	*parse_input(t_list **start)
{
	t_list	*temp;

	if (!start || !*start)
		return (NULL);
	if (join_args_and_redirections(start))
	{
		data()->last_status_code = 1;
		return (NULL);
	}
	temp = *start;
	while (temp)
	{
		add_command_back(&(data()->commands), new_command(new_base_cmd(temp)));
		temp = temp->next;
	}
	ft_lstclear((t_list **)start, free);
	data()->lexer = NULL;
	if (parse_cmd_lst(data()->commands))
		return (NULL);
	parse_expansions(data()->commands);
	parse_builtins(data()->commands);
	return (data()->commands);
}

void	expand(t_command *start)
{
	t_command	*tmp;
	t_quote		*quotes;

	if (!start)
		return ;
	tmp = start;
	while (tmp)
	{
		quotes = init_quotes(count_quotes_pairs(tmp->cmd));
		if (quotes)
			quotes = fill_quotes(tmp->cmd, quotes);
		if (tmp->token != HEREDOC)
			tmp->cmd = expand_env_var(tmp->cmd, quotes);
		if (quotes)
		{
			quotes = fill_quotes(tmp->cmd, quotes);
			tmp->cmd = remove_quotes(tmp->cmd, quotes);
			free(quotes);
		}
		tmp = tmp->next;
	}
}

void	setup_redirs(t_command **start)
{
	int	size;
	int	i;

	if (!start || !*start)
		return ;
	size = count_pipes(*start);
	i = -1;
	pipeline_init(start, size);
	clear_cmd_lst(*start);
	*start = NULL;
	while (i < size)
	{
		i++;
		setup_redirections(&(pipeline())->array[i]);
		setup_outputs_and_inputs(&(pipeline())->array[i]);
	}
	join_pipeline(pipeline()->array, size, start);
	setup_pipes(start);
	free(pipeline()->array);
}

void	stop_assholes(t_command *start)
{
	int			commands;
	t_command	*temp;

	if (!start)
		return ;
	temp = start;
	commands = 0;
	while (temp)
	{
		commands++;
		temp = temp->next;
	}
	if (commands + data()->open_fds >= MAX_FD)
	{
		print_error_messages("my creator doesn't allow you to open"
			" that many file descriptors",
			NULL, WARNING, 0);
		clear_cmd_lst(data()->commands);
		data()->commands = NULL;
	}
}
