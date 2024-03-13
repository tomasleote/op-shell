/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danalmei <danalmei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 10:26:09 by danalmei          #+#    #+#             */
/*   Updated: 2024/03/10 17:52:33 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <utils.h>
#include <types.h>
#include <environment.h>

t_data	*data(void)
{
	static t_data	data;

	return (&data);
}

static void	ignore_args(char **av, int ac)
{
	(void)ac;
	(void)av;
}

void	data_init(char **envp, int ac, char **av)
{
	t_data	*ptr;

	ignore_args(av, ac);
	ptr = data();
	(*env()) = ft_hash_create(8192, NULL, NULL, NULL);
	(*env())->add(*env(), "__env_vars", ft_strdup(""), 's');
	set_env(envp);
	set_shell_level(*env(), (*env())->get_value(*env(), "SHLVL"));
	ptr->lexer = NULL;
	ptr->env_size = 0;
	ptr->open_fds = 1;
	ptr->hidden_code = 0;
	ptr->commands = NULL;
	ptr->last_cmd = NULL;
	ptr->null = open("/dev/null", O_RDWR);
	ptr->env = *env();
	ptr->envp = get_env(*env());
	ptr->is_executing = 0;
}

static void	close_stds(void)
{
	close(1);
	close(0);
	close(data()->null);
}

void	data_destroy(void)
{
	t_data	*ptr;
	int		i;

	ptr = data();
	if (ptr->lexer)
		ft_lstclear(&(ptr->lexer), free);
	if (ptr->commands)
		clear_cmd_lst(ptr->commands);
	if (ptr->last_cmd)
		free(ptr->last_cmd);
	if (ptr->env)
		ft_hash_delete(ptr->env);
	if (ptr->envp)
	{
		i = 0;
		while (i < ptr->env_size)
			free(ptr->envp[i++]);
		free(ptr->envp);
		ptr->envp = NULL;
	}
	close_stds();
	rl_clear_history();
}
