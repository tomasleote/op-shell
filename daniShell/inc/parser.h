/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:22:39 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/11 10:55:01 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <types.h>

//component
void		parser(void);
void		expand(t_command *start);
void		setup_redirs(t_command **start);
void		stop_assholes(t_command *start);
t_command	*parse_input(t_list **start);

//pipeline
void		pipeline_destroy(void);
void		join_pipeline(t_command **array, int size, t_command **start);
t_command	**pipeline_init(t_command **start, int size);
t_command	**create_pipeline(t_command **original, int size);
t_pipeline	*pipeline(void);

//parsing
int			parse_cmd_lst(t_command *start);
void		parse_expansions(t_command *start);
void		parse_builtins(t_command *start);
int			detect_redirection_tokens(t_command *node);
int			join_args(t_list *node, char *content);
int			join_redirections(t_list **start, t_list *node, char *content);
int			join_args_and_redirections(t_list **start);
void		handle_expansion_quotes(t_command *cmd);

//redirections
int			create_redir(t_command **start, t_command *redir, t_command *cmd);
int			find_prev_redirs(t_command **start, t_command *command);
int			find_next_redirs(t_command **start, t_command *command);
int			can_i_execute(t_command **start);
int			setup_redirections(t_command **start);
int			do_i_join(t_command **start);
void		join_stuff(t_command **start);
void		setup_outputs_and_inputs(t_command **start);
void		setup_pipes(t_command **start);
void		dup_ins_outs(t_command *node);
void		set_inputs_and_outs(t_command *node);

//Inputs and outputs
int			set_in(t_command *redir, t_command *cmd, t_command **start);
int			set_out(t_command *redir, t_command *cmd, t_command **start);
int			set_append(t_command **start, t_command *redir, t_command *cmd);
int			set_heredoc(t_command **start, t_command *redir, t_command *cmd);
int			fill_heredoc(int fd, char *eof);
void		run_here_child(int pipe[2], t_command *reddirection);
void		run_here_parent(int pipes[2], t_command **start, t_command *redir,
				t_command *cmd);
void		sig_handle_doc(int signo);

//Expander
char		*extract_env_key(char *start);
char		*swap_env_key(char *str, char *new_str, int *c);
char		*skip_squote(char *str, char *new_str, t_quote *quotes, int *c);
char		*expand_env_var(char *str, t_quote *quote);
char		*expand_env_var_heredoc(char *str);

//Quotes
int			count_quotes_pairs(char *str);
t_quote		*init_quotes(int n_quote);
t_quote		*fill_quotes(char *str, t_quote *quotes);
int			is_to_remove(int index, t_quote *quotes);
char		*remove_quotes(char *str, t_quote *quotes);
int			handle_squote_count(char *str, int *i, int nquote);
int			handle_dquote_count(char *str, int *i, int nquote);

//Utils

int			is_token_redir(t_list *node);
int			is_redir_tok(t_command *cmd);
int			clear_open_fd_memory(char **arr, t_redir *node, t_command *cmd);
char		*special_expansions(char *str, char *new_str, int *c);
char		*add_wrd_to_str(char *str, char *wrd);
char		*add_char_to_str(char *str, char ch);

#endif //PARSER_H
