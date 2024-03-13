/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 23:04:00 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 17:57:15 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include <types.h>

//environment setters and getters
t_hash		**env(void);
void		set_env(char **envp);
void		set_minimum_env(void);
void		set_shell_level(t_hash *table, char *value);
void		update_env(void);
char		**get_env(t_hash *table);

// env util functions used in the builtins
int			add_to_env_vars(char *key);
int			delete_from_env_vars(char *key, t_hash *table);
int			is_key_in_vars(t_hash *table, char *key);
void		reset_env_vars(t_hash *table, char **keys);
char		**get_env_keys(t_hash *table);
char		**create_env(int size, char **keys);

//Prompt
void		set_prompt(char *user);
char		*get_trimmed_cwd(void);
char		*get_hostname(void);
const char	*get_prompt(void);

#endif //ENVIRONMENT_H
