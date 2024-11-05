/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:05:39 by gtraiman          #+#    #+#             */
/*   Updated: 2024/10/19 23:05:39 by gtraiman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
	int				open;
	char			**tab;
    int             expand;
}					t_token;

typedef struct s_data
{
	int ac;
	char **av;
	char **envp;
	char *cwd;
	int exit;
} t_data;

typedef struct s_env {
    char *name;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_cmd_list {
    char        **files_list;
    int         *files_type;
    int         last_in;
    int         last_out;
    int         files_count;
    int         open;
    char        **cmd_args;
    char        *cmd;
    t_env   *env_vars;
    struct s_cmd_list  *next;
} t_cmd_list;

# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include "parsing/parsing.h"
# include "exec/builtins/builtins.h"
# include "libft/libft.h"
# include "exec/exec.h"
# include <signal.h>
# include "signals/signals.h"
#include <readline/history.h>
#include <readline/readline.h>


#endif