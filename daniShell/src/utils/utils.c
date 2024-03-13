/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:42:31 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/03 19:28:46 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utils.h>

char	*str_joiner(char *s1, char *s2, int flag)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	if (!flag)
		return (res);
	else if (flag == 1 && s1)
		free(s1);
	else if (flag == 2 && s2)
		free(s2);
	else if (flag == 3)
	{
		if (s1)
			free(s1);
		if (s2)
			free(s2);
	}
	return (res);
}

static int	is_dot_or_dot_dot(char *str)
{
	if (ft_strncmp(str, ".", 1) == 0 || ft_strncmp(str, "..", 2) == 0)
		return (1);
	return (0);
}

char	*get_cmd_path(char *cmd, char *path)
{
	char	*res;
	char	*temp1;
	char	**temp;
	char	**paths;
	int		i;

	if (!path || !cmd || ft_strlen(cmd) == 0)
		return (NULL);
	temp = ft_split(cmd, 0x1F);
	if (temp[0] && is_dot_or_dot_dot(temp[0]))
		return (ft_fsplit(temp), NULL);
	paths = ft_split(path, ':');
	i = -1;
	while (paths[++i])
	{
		temp1 = str_joiner(paths[i], "/", 0);
		res = str_joiner(temp1, temp[0], 1);
		if (access(res, F_OK) == 0)
			return (ft_fsplit(temp), ft_fsplit(paths), res);
		free(res);
	}
	ft_fsplit(temp);
	ft_fsplit(paths);
	return (NULL);
}

char	*show_token(t_token token)
{
	if (token == PIPE)
		return (BLUE"PIPE"WHITE);
	else if (token == OUT)
		return (BLUE"OUT"WHITE);
	else if (token == IN)
		return (BLUE"IN"WHITE);
	else if (token == HEREDOC)
		return (BLUE"HEREDOC"WHITE);
	else if (token == APPEND)
		return (BLUE"APPEND"WHITE);
	else if (token == COMMAND)
		return (BLUE"COMMAND"WHITE);
	else if (token == DQOUTE)
		return (BLUE"DQUOTE"WHITE);
	else if (token == QOUTE)
		return (BLUE"QUOTE"WHITE);
	else if (token == NIL)
		return (RED"NIL"WHITE);
	return (NULL);
}

char	*show_bool(bool boolean)
{
	if (boolean == true)
		return (GREEN"true"WHITE);
	else if (boolean == false)
		return (RED"false"WHITE);
	return (NULL);
}
