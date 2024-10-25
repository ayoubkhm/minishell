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
}					t_token;

typedef struct s_data
{
	int ac;
	char **av;
	char **envp;
	char *cwd;
	int exit;
} t_data;

typedef struct s_cmd_list // un noeud de la liste = une commande, exemple : cat | ls  1 noeud pour cat et un deuxieme noeud pour ls
{
	char	**files_list; // liste infile et outfile dans l'ordre
	int		*files_type;   // 0 pour infile 1 pour outfile
	int			last_in;       // index du dernier infile
	int			last_out;      // index du dernier outfile
	int 	files_count; // Nouveau champ pour suivre le nombre de redirections
	int	open;
    char        **cmd_args;     // Tableau de la commande et de ses arguments
	char		*cmd;         // string de la commande ex : "ls -a"
	struct s_cmd_list	*next;
} 		t_cmd_list;



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


#endif