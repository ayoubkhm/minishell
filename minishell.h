/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/10/19 23:05:39 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/19 23:05:39 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "exec/builtins/builtins.h"
# include "exec/exec.h"
# include "libft/libft.h"
# include "parsing/parsing.h"
# include "signals/signals.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
	int				open;
	char			**tab;
	int				expand;
	int				is_var;
}	t_token;

typedef struct s_data
{
	int		ac;
	char	**av;
	char	**envp;
	char	*cwd;
	int		exit;
	int		nodenb;
	int		prev_pipe_read_end;

}	t_data;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				exit_status;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd_list
{
	char				**files_list;
	int					*files_type;
	int					last_in;
	int					last_out;
	int					files_count;
	int					open[2];
	int					save_std[2];
	char				**cmd_args;
	int					pipe[2];
	char				*cmd;
	int					error;
	t_env				*env_vars;
	char				*heredoc_delimiter;
	char				*heredoc_content;
	struct s_cmd_list	*next;
	struct s_cmd_list	*prev;
}	t_cmd_list;

typedef struct s_var_exp_params
{
	char	*input;
	int		*index;
	t_token	**tokens;
	t_env	*env_list;
}	t_var_exp_params;

typedef struct s_var_exp
{
	char	*input;
	int		*index;
	int		in_quotes;
	t_token	**tokens;
	t_env	*env_list;
}	t_var_exp;

typedef struct s_variable_data
{
	char	*name;
	char	*val;
	int		start;
	int		val_start;
	int		end;
}	t_variable_data;

typedef struct s_quote_context
{
	char	*input;
	int		*i;
	t_env	*env_list;
	char	*final_content;
}	t_quote_context;

typedef struct s_ctx
{
	char	*inp;
	int		i;
	t_token	**tok;
	t_env	*e_l;
	char	*prefix;
	char	*dol_seq;
	int		dol_cnt;
}	t_ctx;

typedef struct s_pars_cxt
{
	char	*inp;
	int		i;
	char	*pfx;
	t_token	**tok;
	t_env	*e_l;
}	t_pars_cxt;

#endif