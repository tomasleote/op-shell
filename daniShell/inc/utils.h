/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:40:59 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/03 18:44:54 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <libft.h>
# include <stdbool.h>
# include <types.h>

//Printing utils
char		*show_bool(bool boolean);
char		*show_token(t_token token);

//ft utils
char		*str_joiner(char *s1, char *s2, int flag);
char		*ft_strstr(char *haystack, char *needle);
char		*get_cmd_path(char *cmd, char *path);
char		*extract_key(const char *arg);

// Command list utils
int			count_pipes(t_command *node);
int			cmd_lst_size(t_command *node);
void		delete_cmd_node(t_command *node);
void		clear_cmd_lst(t_command *node);
void		print_cmd(t_command *node);
void		print_cmd_lst(t_command *node);
void		remove_node_cmdlst(t_command **start, t_command *node);
void		move_node_to_lst(t_command **dst, t_command *to_move);
void		add_node_between_cmds(t_command **start, t_command *to_add,
				t_command *new);
t_command	new_empty_cmd(void);
t_command	new_base_cmd(t_list *node);
t_command	*new_command(t_command command);
t_command	*clone_command(t_command *command);
t_command	*add_command_back(t_command **start, t_command *command);

// Redirection list utils
void		clear_redir_lst(t_redir *node);
void		print_redir_lst(t_redir *node);
void		remove_node_redir_lst(t_redir **start, t_redir *node);
t_redir		new_base_redir(t_command *cmd);
t_redir		*new_redir(t_redir redirection);
t_redir		*add_redir_back(t_redir **start, t_redir *node);

//Parsing Utils
int			is_ws_or_empty(char *str, int flag);
int			is_builtin(char *str);
int			is_whitespace(char c);
int			is_quote(char c);
int			is_squote(char c);
int			is_dquote(char c);
int			is_pipe(char c);
int			is_heredoc(char c, char next);
int			is_append(char c, char next);
int			is_in(char c, char next);
int			is_out(char c, char next);
int			is_dollar(char c);
int			redir_lookup(char *str, int pos, int size);

//Error handling
void		print_error_messages(char *message, char *argument,
				t_error type, int flag);
int			check_max_fds(void);
#endif //UTILS_H
