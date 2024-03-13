/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:29:50 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/08 18:07:31 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <libft.h>
# include <stdbool.h>
# include <ft_hash.h>
# include <termios.h>

# define MAX_FD	1010

typedef struct s_buff_ptr	t_buff_ptr;
typedef struct s_builtins	t_builtins;
typedef struct s_pipeline	t_pipeline;
typedef struct s_command	t_command;
typedef struct s_buffer		t_buffer;
typedef struct s_redir		t_redir;
typedef struct s_quote		t_quote;
typedef struct s_data		t_data;
typedef struct s_fds		t_fds;
typedef enum e_token		t_token;
typedef enum e_error		t_error;

struct s_buffer {
	const char	*input;
	char		*buffer;
};

struct s_quote {
	char	quote;
	int		start;
	int		end;
};

struct s_buff_ptr {
	int	i;
	int	k;
};

struct s_fds {
	int	infile;
	int	outfile;
};

enum e_error {
	SYNTAX,
	CD,
	ENV,
	EXIT,
	WARNING,
};

enum e_token {
	PIPE,
	OUT,
	IN,
	DQOUTE,
	QOUTE,
	HEREDOC,
	APPEND,
	COMMAND,
	NIL
};

struct s_redir {
	t_token	type;
	t_fds	fds;
	t_redir	*prev;
	t_redir	*next;
};

struct s_command {
	char		*cmd_path;
	char		*cmd;
	bool		is_builtin;
	bool		expand;
	bool		pipe;
	int			pipes[2];
	pid_t		pid;
	t_fds		redirections;
	t_token		token;
	t_redir		*redirs;
	t_command	*prev;
	t_command	*next;
};

struct s_pipeline {
	t_command	**array;
	t_command	*temp;
	pid_t		pid;
	char		**char_array;
	t_fds		temp_fds;
	int			size;
};

struct s_data {
	int				env_size;
	char			*last_cmd;
	pid_t			last_pid;
	int				last_status_code;
	t_hash			*env;
	char			**envp;
	t_list			*lexer;
	t_command		*commands;
	int				open_fds;
	int				null;
	int				hidden_code;
	bool			is_executing;
	bool			on_heredoc;
	bool			exited_on_heredoc;
	bool			sigint_recived;
};

struct	s_builtins {
	int	(*echo)(char **args);
	int	(*cd)(char **args);
	int	(*unset)(char **args);
	int	(*export)(char **args);
	int	(*exit)(char **args, t_command *node);
	int	(*pwd)(char **args);
	int	(*env)(char **args);
};

#endif //TYPES_H