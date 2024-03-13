/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 22:03:28 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:04:15 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <utils.h>
# include <environment.h>

// Component
void	executioner(void);
int		is_valid_list(t_command **start);
void	execute_list(t_command *start);

// Execution Utils
int		get_correct_code(char *arg, int code);
int		is_directory(char *path);
void	execute_token_is_command(t_command *tmp);
void	execute_token_is_nil(t_command *tmp);
void	execute_cmd(t_command *tmp);
void	child_execution(t_command *cmd);
void	execute_has_cmd_path(t_command *cmd);
void	execute_has_no_cmd_path(t_command *cmd);
void	execute_no_args(t_command *cmd);
void	execute_is_builtin(t_command *cmd);
void	handle_builtin_execution(t_command *node);
void	handle_execve_error(t_command *node, char **arr, int code);
void	open_pipeline(t_command *start);
void	close_pipeline(t_command *start);
void	close_fds(t_command *start);
void	redirect_stds(t_command *tmp);
void	restore_stds(int fds[2]);
void	close_curr_fds(t_command *tmp);
void	put_err_msg(char *arg, char *message);
void	can_i_make_exec(t_command *node);
void	wait_processes(t_command *start);
void	copy_default_stds(t_command *cmd, int fds[2]);
void	replace_ws_for_space(t_command *node);
char	*get_err_msg(int code, char *arg);

// Builtins
int		builtins(t_command *node);
int		execute_builtins(char *builtin, char **args, t_command *node);
int		ft_cd(char **args);
int		ft_pwd(char **args);
int		ft_env(char **args);
int		ft_echo(char **args);
int		ft_exit(char **args, t_command *node);
int		ft_unset(char **args);
int		ft_export(char **args);

// Builtin Utils
int		ft_access(char *arg);
int		ft_quit(char **args, int code, char *message, t_command *node);
int		add_to_environment(char *arg);
int		is_valid_key(char *key);
int		handle_export_errors(char *arg);
int		add_args_if_possible(char **args);
int		is_string(char *str);
int		count_args(char **args);
int		going_home(char *pwd, char *cwd);
int		check_cd_args(char **args, char *cwd);
int		is_same_file(int fd1, int fd2);
void	add_key(char *key, char *value);
void	print_export(void);

#endif //EXECUTION_H
