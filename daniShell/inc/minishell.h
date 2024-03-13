/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fparreir <fparreir@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/09 22:03:28 by fparreir          #+#    #+#             */
/*   Updated: 2024/03/10 18:03:45 by fparreir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <ft_hash.h>
# include <libft.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <termios.h>
# include <errno.h>
# include <signal.h>
# include <types.h>

// Global data
t_data		*data(void);
void		data_destroy(void);
void		data_init(char **envp, int ac, char **av);

// Signals
void		reset_prompt(void);
void		sigint_handler(int signo);
void		setup_signal_handlers(void);

#endif //MINISHELL_H
